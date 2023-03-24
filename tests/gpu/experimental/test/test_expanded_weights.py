from dataclasses import dataclass
from functools import partial
from itertools import product, chain
import unittest
import torch
import torch.nn as nn
import torch.nn.functional as F
from torch.nn import CrossEntropyLoss
from torch.nn.utils._per_sample_grad import call_for_per_sample_grads
from torch.testing._internal.common_cuda import TEST_CUDA
from torch.testing._internal.common_device_type import OpDTypes, instantiate_device_type_tests, ops
from torch.testing._internal.common_nn import TestBase, module_tests, new_module_tests
from torch.testing._internal.common_utils import TestCase, freeze_rng_state, make_tensor, run_tests, parametrize
from torch.testing._internal.common_methods_invocations import SampleInput, op_db
from torch.nn.utils._expanded_weights import ExpandedWeight
from torch.nn.utils._expanded_weights.expanded_weights_utils import forward_helper, set_grad_sample_if_exists, unpack_expanded_weight_or_tensor, sum_over_all_but_batch_and_last_n, standard_kwargs
from common.pytorch_test_base import TestCase, dtypesIfXPU, TEST_XPU, TEST_MULTIGPU, largeTensorTest
from common.common_nn import TestBase, module_tests, new_module_tests

class TestContext:
    pass

class TestExpandedWeightHelperFunction(TestCase):

    def test_forward_helper(self, device):
        input = torch.randn(3, 4, device=device)
        weight = torch.randn(5, 4, device=device)
        bias = torch.randn(5, device=device)
        for (weight_batched, bias_batched) in product([True, False], [True, False]):
            maybe_batched_weight = weight
            maybe_batched_bias = bias
            if weight_batched:
                maybe_batched_weight = ExpandedWeight(weight.clone().requires_grad_(), 3, loss_reduction='sum')
            if bias_batched:
                maybe_batched_bias = ExpandedWeight(bias.clone().requires_grad_(), 3, loss_reduction='sum')
            args = (input, maybe_batched_weight, maybe_batched_bias)
            (expanded_args, expanded_kwargs) = standard_kwargs(('bias',), args)
            res = forward_helper(nn.functional.linear, expanded_args, expanded_kwargs)
            expected = nn.functional.linear(input, weight, bias)
            self.assertEqual(res, expected)
            self.assertEqual(len(expanded_args), 2)
            assert expanded_args[0] is args[0]
            assert expanded_args[1] is args[1]
            self.assertEqual(len(expanded_kwargs), 1)
            assert expanded_kwargs['bias'] is args[2]

    def test_forward_helper_failure_args(self, device):
        weight = torch.randn(5, 4, device=device)
        bias = torch.randn(5, device=device)
        with self.assertRaisesRegex(RuntimeError, 'do not support inputs that are also ExpandedWeights.'):
            input = ExpandedWeight(torch.randn(3, 4, requires_grad=True), 3, loss_reduction='sum')
            (expanded_args, expanded_kwargs) = standard_kwargs(('bias',), (input, weight, bias))
            forward_helper(nn.functional.linear, expanded_args, expanded_kwargs)
        with self.assertRaisesRegex(RuntimeError, 'requires a Tensor as the first input'):
            (expanded_args, expanded_kwargs) = standard_kwargs(('bias',), (3, weight, bias))
            forward_helper(nn.functional.linear, expanded_args, expanded_kwargs)
        with self.assertRaisesRegex(RuntimeError, 'requires a batch dimension but got an input of size 0'):
            (expanded_args, expanded_kwargs) = standard_kwargs(('bias',), (torch.tensor(3), weight, bias))
            forward_helper(nn.functional.linear, expanded_args, expanded_kwargs)
        with self.assertRaisesRegex(RuntimeError, '0 is not a valid batch size for Expanded Weights'):
            (expanded_args, expanded_kwargs) = standard_kwargs(('bias',), (torch.randn(0, 1, 2), weight, bias))
            forward_helper(nn.functional.linear, expanded_args, expanded_kwargs)
        input = torch.randn(3, 4)
        for (weight_batched, bias_batched) in product([True, False], [True, False]):
            if not weight_batched and (not bias_batched):
                continue
            maybe_batched_weight = weight
            maybe_batched_bias = bias
            if weight_batched:
                maybe_batched_weight = ExpandedWeight(weight.clone().requires_grad_(), 4, loss_reduction='sum')
            if bias_batched:
                maybe_batched_bias = ExpandedWeight(bias.clone().requires_grad_(), 4, loss_reduction='sum')
            with self.assertRaisesRegex(RuntimeError, 'Expected ExpandedWeights to have batch size matching input'):
                (expanded_args, expanded_kwargs) = standard_kwargs(('bias',), (input, maybe_batched_weight, maybe_batched_bias))
                forward_helper(nn.functional.linear, expanded_args, expanded_kwargs)

    def test_set_grad_sample_if_exists(self, device):

        def test_fn(a):
            return True
        orig_weight = torch.randn(4, device=device, requires_grad=True)
        expanded_weight = ExpandedWeight(orig_weight, 3, loss_reduction='sum')
        set_grad_sample_if_exists(expanded_weight, test_fn)
        self.assertTrue(hasattr(orig_weight, 'grad_sample'))
        self.assertTrue(orig_weight.grad_sample)
        basic_tensor = torch.randn(4, device=device)
        set_grad_sample_if_exists(basic_tensor, test_fn)
        self.assertFalse(hasattr(basic_tensor, 'grad_sample'))
        non_tensor = 3
        set_grad_sample_if_exists(non_tensor, test_fn)
        self.assertFalse(hasattr(non_tensor, 'grad_sample'))

    def test_set_grad_sample_if_exists_failure(self, device):

        def test_fn(a):
            return True
        grad_tensor = torch.randn(4, requires_grad=True, device=device)
        with self.assertRaisesRegex(RuntimeError, 'does not support a mixture of ExpandedWeight parameters and normal Parameters'):
            set_grad_sample_if_exists(grad_tensor, test_fn)

    def test_unpack_expanded_weight_or_tensor(self, device):
        input = torch.randn(3, requires_grad=True, device=device)
        self.assertEqual(input, unpack_expanded_weight_or_tensor(ExpandedWeight(input, 3, loss_reduction='sum')))
        input.requires_grad_(False)
        self.assertEqual(input, unpack_expanded_weight_or_tensor(input))
        self.assertTrue(unpack_expanded_weight_or_tensor(4) is None)

    def test_unpack_expanded_weight_or_tensor_with_custom_function(self, device):
        input = torch.randn(3, requires_grad=True, device=device)
        self.assertTrue(unpack_expanded_weight_or_tensor(ExpandedWeight(input, 3, loss_reduction='sum'), lambda x: x is input))
        input.requires_grad_(False)
        self.assertTrue(unpack_expanded_weight_or_tensor(input, lambda x: x is input))
        self.assertTrue(unpack_expanded_weight_or_tensor(4, lambda x: x is input) is None)

    def test_unpack_expanded_weight_or_tensor_failure(self, device):
        input = torch.randn(3, requires_grad=True, device=device)
        with self.assertRaisesRegex(RuntimeError, 'does not support a mixture of ExpandedWeight parameters and normal Parameters'):
            unpack_expanded_weight_or_tensor(input)
        with self.assertRaisesRegex(RuntimeError, 'does not support a mixture of ExpandedWeight parameters and normal Parameters'):
            unpack_expanded_weight_or_tensor(input, lambda x: x is input)

    def test_sum_over_all_but_batch_and_last_n(self, device):
        input = torch.randn(1, 2, 3, 4, 5, device=device)
        res = sum_over_all_but_batch_and_last_n(input, 2)
        expected = input.sum((1, 2))
        self.assertEqual(res, expected)
        res = sum_over_all_but_batch_and_last_n(input, 0)
        expected = input.sum((1, 2, 3, 4))
        self.assertEqual(res, expected)
        res = sum_over_all_but_batch_and_last_n(input, 4)
        self.assertEqual(res, input)

class TestExpandedWeightFunctional(TestCase):

    def _compare_ew_and_for_loop_per_sample_grads(self, op, sample_input, reduction):
        input = sample_input.input
        args = sample_input.args
        kwargs = sample_input.kwargs
        batch_size = input.shape[0] if len(input.shape) > 1 else 1
        loss_reduction = 'sum' if reduction == torch.sum else 'mean'
        (ew_input, ew_args, ew_kwargs) = make_expanded_weight(sample_input, batch_size, loss_reduction)
        diff_input_list = (ew_input,) + tuple(ew_args) + tuple(ew_kwargs.values())
        diff_input_list = [i for i in diff_input_list if is_diff_tensor(i)]
        diff_input_list = [i.orig_weight if isinstance(i, ExpandedWeight) else i for i in diff_input_list]
        if not diff_input_list:
            return
        result = run_op(op, ew_input, *ew_args, **ew_kwargs)
        reduction(result).backward()
        expanded_weight_grad = tuple((i.grad_sample if hasattr(i, 'grad_sample') else i.grad for i in diff_input_list))
        func = partial(run_op, op)
        per_sample_grad = for_loop_per_sample_grad(batch_size, reduction, input, func, *args, **kwargs)
        self.assertEqual(len(per_sample_grad), len(expanded_weight_grad))
        if loss_reduction == 'mean':
            expanded_weight_grad = expanded_weight_grad[1:]
            per_sample_grad = per_sample_grad[1:]
        for (result_grad, expected_grad) in zip(expanded_weight_grad, per_sample_grad):
            self.assertEqual(result_grad, expected_grad)

    @ops(filter(lambda op: op.supports_expanded_weight, op_db), dtypes=OpDTypes.supported, allowed_dtypes=(torch.double,))
    def test_expanded_weight_per_sample_grad_sum(self, device, dtype, op):
        sample_inputs = op.sample_inputs(device, dtype, requires_grad=True)
        for sample_input in supported_inputs(op, sample_inputs):
            if op.name == 'nn.functional.embedding':
                sample_input = SampleInput(sample_input.args[0], args=(sample_input.input,), kwargs=sample_input.kwargs)
            self._compare_ew_and_for_loop_per_sample_grads(op, sample_input, torch.sum)

    @ops(filter(lambda op: op.supports_expanded_weight, op_db), dtypes=OpDTypes.supported, allowed_dtypes=(torch.double,))
    def test_expanded_weight_per_sample_grad_mean(self, device, dtype, op):
        sample_inputs = op.sample_inputs(device, dtype, requires_grad=True)
        for sample_input in supported_inputs(op, sample_inputs):
            if op.name == 'nn.functional.embedding':
                sample_input = SampleInput(sample_input.args[0], args=(sample_input.input,), kwargs=sample_input.kwargs)
            self._compare_ew_and_for_loop_per_sample_grads(op, sample_input, torch.mean)

    @ops(filter(lambda op: op.supports_expanded_weight, op_db), dtypes=OpDTypes.supported, allowed_dtypes=(torch.double,))
    def test_expanded_weights_per_sample_grad_input_no_grad(self, device, dtype, op):
        sample_inputs = op.sample_inputs(device, dtype, requires_grad=True)
        for sample_input in supported_inputs(op, sample_inputs):
            if op.name == 'nn.functional.embedding':
                sample_input = SampleInput(sample_input.args[0], args=(sample_input.input,), kwargs=sample_input.kwargs)
            sample_input.input.requires_grad_(False)
            self._compare_ew_and_for_loop_per_sample_grads(op, sample_input, torch.mean)

    @ops(filter(lambda op: op.supports_expanded_weight, op_db), dtypes=OpDTypes.supported, allowed_dtypes=(torch.double,))
    def test_unsupported_expand_weights(self, device, dtype, op):
        sample_inputs = op.sample_inputs(device, dtype, requires_grad=True)
        unsupported_inputs = supported_inputs(op, sample_inputs, supported_inputs=False)
        for sample_input in unsupported_inputs:
            with self.assertRaisesRegex(RuntimeError, 'Expanded Weights'):
                if op.name == 'nn.functional.embedding':
                    sample_input = SampleInput(sample_input.args[0], args=(sample_input.input,), kwargs=sample_input.kwargs)
                input = sample_input.input
                batch_size = input.shape[0] if len(input.shape) > 1 else 1
                (ew_input, ew_args, ew_kwargs) = make_expanded_weight(sample_input, batch_size)
                result = run_op(op, ew_input, *ew_args, **ew_kwargs)
                diff_input_list = (ew_input,) + tuple(ew_args) + tuple(ew_kwargs.values())
                diff_input_list = [i for i in diff_input_list if is_diff_tensor(i)]
                diff_input_list = [i.orig_weight if isinstance(i, ExpandedWeight) else i for i in diff_input_list]
                result.sum().backward()

    @ops(filter(lambda op: op.supports_expanded_weight, op_db), dtypes=OpDTypes.supported)
    def test_expanded_weight_forward(self, device, dtype, op):
        sample_inputs = op.sample_inputs(device, dtype)
        for sample_input in supported_inputs(op, sample_inputs):
            if op.name == 'nn.functional.embedding':
                sample_input = SampleInput(sample_input.args[0].clone(), args=(sample_input.input.clone(),), kwargs=sample_input.kwargs)
                if 'xpu' in device and 'max_norm' in sample_input.kwargs and ('padding_idx' in sample_input.kwargs):
                    self.skipTest('embedding is non-determinstic in this case, see issue #74679')
            batch_size = sample_input.input.shape[0] if len(sample_input.input.shape) > 1 else 1
            for loss_reduction in ['sum', 'mean']:
                (ew_input, ew_args, ew_kwargs) = make_expanded_weight(sample_input, batch_size, loss_reduction)
                expanded_weight_result = run_op(op, ew_input, *ew_args, **ew_kwargs)
                normal_result = run_op(op, sample_input.input, *sample_input.args, **sample_input.kwargs)
                self.assertEqual(expanded_weight_result, normal_result)

    def test_expanded_weight_error(self, device):
        batch_size = 3
        sample_input = make_tensor((batch_size, 4), dtype=torch.float32, device=device, requires_grad=True)
        sample_weight = make_tensor(4, dtype=torch.float32, device=device, requires_grad=True)
        with self.assertRaisesRegex(RuntimeError, 'Expanded Weights encountered but cannot handle function'):
            torch.add(sample_input, ExpandedWeight(sample_weight, batch_size, loss_reduction='sum'))

    def _test_embedding_model(self, model, num_embedding, device):
        batch_size = 32
        input = torch.randint(0, num_embedding, (batch_size, 5, 5), device=device)
        return self._test_model(partial(model, num_embedding=num_embedding), batch_size, input, device)

    def _test_conv_model(self, model, input_size, num_dim, device, loss_reduction='sum'):
        batch_size = 32
        input_ending = [input_size] * num_dim
        input = torch.randn([batch_size, 3] + input_ending, device=device)
        return self._test_model(partial(model, num_dim=num_dim), batch_size, input, device, loss_reduction)

    def _test_model(self, model, batch_size, input, device, loss_reduction='sum'):
        model = model(10).to(device)
        targets = torch.randint(0, 10, (batch_size,), device=device)
        criterion = CrossEntropyLoss(reduction=loss_reduction)
        result = call_for_per_sample_grads(model, loss_reduction=loss_reduction)(input)
        loss = criterion(result, targets)
        loss.backward()
        result = []
        for weight in model.parameters():
            result.append(weight.grad_sample)
            del weight.grad_sample
        expected = []
        for i in range(batch_size):
            loss = criterion(model(input[i].unsqueeze(0)), targets[i].unsqueeze(0))
            expected.append(torch.autograd.grad(loss, model.parameters(), torch.ones_like(loss)))
        expected = [torch.stack(grad) for grad in zip(*expected)]
        for (res, exp) in zip(result, expected):
            self.assertEqual(res, exp, atol=0.0001, rtol=5e-05)

    def test_cnn_model_sum(self, device):

        def convnet(num_classes, num_dim):
            return nn.Sequential(nn.Conv2d(3, 32, kernel_size=3, stride=1, padding=1), nn.ReLU(), nn.AvgPool2d(kernel_size=2, stride=2), nn.Conv2d(32, 64, kernel_size=3, stride=1, padding=1), nn.ReLU(), nn.AvgPool2d(kernel_size=2, stride=2), nn.Conv2d(64, 64, kernel_size=3, stride=1, padding=1), nn.ReLU(), nn.AvgPool2d(kernel_size=2, stride=2), nn.Conv2d(64, 128, kernel_size=3, stride=1, padding=1), nn.ReLU(), nn.AdaptiveAvgPool2d((1, 1)), nn.Flatten(start_dim=1, end_dim=-1), nn.Linear(128, num_classes, bias=True))
        return self._test_conv_model(convnet, 28, 2, device)

    def test_cnn_model_mean(self, device):

        def convnet(num_classes, num_dim):
            return nn.Sequential(nn.Conv2d(3, 32, kernel_size=3, stride=1, padding=1), nn.ReLU(), nn.AvgPool2d(kernel_size=2, stride=2), nn.Conv2d(32, 64, kernel_size=3, stride=1, padding=1), nn.ReLU(), nn.AvgPool2d(kernel_size=2, stride=2), nn.Conv2d(64, 64, kernel_size=3, stride=1, padding=1), nn.ReLU(), nn.AvgPool2d(kernel_size=2, stride=2), nn.Conv2d(64, 128, kernel_size=3, stride=1, padding=1), nn.ReLU(), nn.AdaptiveAvgPool2d((1, 1)), nn.Flatten(start_dim=1, end_dim=-1), nn.Linear(128, num_classes, bias=True))
        return self._test_conv_model(convnet, 28, 2, device, loss_reduction='mean')

    @parametrize('num_dim', [1, 2, 3])
    def test_instance_norm_model(self, num_dim, device):

        def instance_norm_model(num_classes, num_dim):
            conv_layer = nn.Conv1d if num_dim == 1 else nn.Conv2d if num_dim == 2 else nn.Conv3d
            norm_layer = nn.InstanceNorm1d if num_dim == 1 else nn.InstanceNorm2d if num_dim == 2 else nn.InstanceNorm3d
            return nn.Sequential(conv_layer(3, 32, kernel_size=3, stride=1, padding=1), norm_layer(32, affine=True), nn.Flatten(start_dim=1, end_dim=-1), nn.Linear(32 * 7 ** num_dim, num_classes, bias=True))
        return self._test_conv_model(instance_norm_model, 7, num_dim, device)

    @parametrize('num_dim', [1, 2, 3])
    def test_group_norm_model(self, num_dim, device):

        def group_norm_model(num_classes, num_dim):
            conv_layer = nn.Conv1d if num_dim == 1 else nn.Conv2d if num_dim == 2 else nn.Conv3d
            return nn.Sequential(conv_layer(3, 32, kernel_size=3, stride=1, padding=1), nn.GroupNorm(8, 32, affine=True), nn.Flatten(start_dim=1, end_dim=-1), nn.Linear(32 * 7 ** num_dim, num_classes, bias=True))
        return self._test_conv_model(group_norm_model, 7, num_dim, device)

    @parametrize('num_dim', [1, 2, 3])
    def test_layer_norm_model(self, num_dim, device):

        def layer_norm_model(num_classes, num_dim):
            conv_layer = nn.Conv1d if num_dim == 1 else nn.Conv2d if num_dim == 2 else nn.Conv3d
            normalized_shape = [7] * num_dim
            return nn.Sequential(conv_layer(3, 32, kernel_size=3, stride=1, padding=1), nn.LayerNorm(normalized_shape, elementwise_affine=True), nn.Flatten(start_dim=1, end_dim=-1), nn.Linear(32 * 7 ** num_dim, num_classes, bias=True))
        return self._test_conv_model(layer_norm_model, 7, num_dim, device)

    def test_embedding_model(self, device):

        def embedding_model(num_classes, num_embedding):
            return nn.Sequential(nn.Embedding(num_embedding, 15), nn.Flatten(start_dim=1, end_dim=-1), nn.Linear(375, num_classes, bias=True))
        return self._test_embedding_model(embedding_model, 16, device)

    def test_group_norm_error(self, device):
        N = 3
        C = 5
        inp = torch.randn(N, C)
        with self.assertRaisesRegex(RuntimeError, 'Expected number of channels in input to be divisible'):
            F.group_norm(inp, 2)

class TestExpandedWeightModule(TestCase):

    def _do_test(self, module, input):
        batch_size = input.shape[0]
        diff_input = input.dtype == torch.float or input.dtype == torch.double
        if diff_input:
            input.requires_grad_()
        with freeze_rng_state():
            actual_res = call_for_per_sample_grads(module, loss_reduction='sum')(input).sum()
            actual_res.backward()
            actual_grads = []
            for param in module.parameters():
                actual_grads.append(param.grad_sample)
                del param.grad_sample
            if diff_input:
                actual_grads.append(input.grad.clone())
                input.grad = torch.zeros_like(input.grad)
            expected_res = torch.tensor(0.0, device=input.device, dtype=torch.double)
            expected_grads = []
            for i in range(batch_size):
                input_slice = input[i]
                diff_params = module.parameters()
                if diff_input:
                    diff_params = chain(diff_params, (input_slice,))
                res = module(input_slice.unsqueeze(0)).sum()
                out_grads = torch.autograd.grad(res, diff_params, torch.ones_like(res), allow_unused=True)
                expected_grads.append(out_grads)
                expected_res += res
            expected_grads = tuple((torch.stack(grad) for grad in zip(*expected_grads)))
        self.assertEqual(actual_res, expected_res)
        [self.assertEqual(actual, expected) for (actual, expected) in zip(actual_grads, expected_grads)]

    def _do_test_multi_input(self, module, input):

        class TestModule(nn.Module):

            def __init__(self, module):
                super().__init__()
                self.module = module

            def forward(self, input):
                return self.module(input) + self.module(input)
        batch_size = input.shape[0]
        diff_input = input.dtype == torch.float or input.dtype == torch.double
        if diff_input:
            input.requires_grad_()
        with freeze_rng_state():
            test_module = TestModule(module)
            actual_res = call_for_per_sample_grads(test_module, loss_reduction='sum')(input).sum()
            actual_res.backward()
            actual_grads = []
            for param in module.parameters():
                actual_grads.append(param.grad_sample)
                del param.grad_sample
            if diff_input:
                actual_grads.append(input.grad.clone())
                input.grad = torch.zeros_like(input.grad)
            expected_grads = []
            for i in range(batch_size):
                input_slice = input[i]
                diff_params = module.parameters()
                if diff_input:
                    diff_params = chain(diff_params, (input_slice,))
                res = module(input_slice.unsqueeze(0)).sum()
                out_grads = torch.autograd.grad(res, diff_params, torch.ones_like(res), allow_unused=True)
                expected_grads.append(out_grads)
        expected_grads = tuple((torch.stack(grad) for grad in zip(*expected_grads)))
        expected_grads = tuple((expected_grad for expected_grad in expected_grads if expected_grad is not None))
        assert [self.assertEqual(actual, 2 * expected) for (actual, expected) in zip(actual_grads, expected_grads)]

    def test_per_sample_api_failing(self):
        module = nn.Linear(10, 10)
        input = torch.randn(64, 10)
        with self.assertRaisesRegex(RuntimeError, 'Module passed must be nn.Module'):
            call_for_per_sample_grads('fail')(input)
        with self.assertRaisesRegex(RuntimeError, 'Batch size passed must be None or an integer'):
            call_for_per_sample_grads(module, batch_size=6.4)(input)
        with self.assertRaisesRegex(RuntimeError, 'Batch size must be positive'):
            call_for_per_sample_grads(module, batch_size=-64)(input)
        with self.assertRaisesRegex(RuntimeError, 'incorrect for multiple calls'):
            loss = call_for_per_sample_grads(module)(input).sum()
            loss.backward()
            call_for_per_sample_grads(module)(input)
        module = nn.Linear(10, 10)
        with self.assertRaisesRegex(RuntimeError, 'Expected loss_reduction argument to be sum or mean'):
            call_for_per_sample_grads(module, loss_reduction='')(input)

    def test_per_sample_api_compute_batch_size(self):

        class CustomModule(nn.Module):

            def __init__(self):
                super().__init__()
                self.linear = nn.Linear(5, 5)

            def forward(self, input1, input2):
                return self.linear(input1) + self.linear(input2)
        module = CustomModule()
        input1 = torch.randn(4, 5)
        input2 = torch.randn(5, 5)
        with self.assertRaisesRegex(RuntimeError, 'found at least one input with batch size 4 and one with batch size 5'):
            call_for_per_sample_grads(module)(input1, input2)
        input2 = torch.randn(4, 5)
        call_for_per_sample_grads(module)(input1, input2)
        module = CustomModule()
        call_for_per_sample_grads(module)(input1, input2=input2)
        module = CustomModule()
        call_for_per_sample_grads(module)(input1=input1, input2=input2)

    def test_per_sample_api_compute_batch_size_not_pytreeable(self):

        @dataclass
        class NonPytreeableTuple:
            elem1: torch.Tensor
            elem2: torch.Tensor

        class CustomModule(nn.Module):

            def __init__(self):
                super().__init__()
                self.linear = nn.Linear(5, 5)

            def forward(self, input1, input2):
                return self.linear(input1.elem1) + self.linear(input1.elem2)
        input = NonPytreeableTuple(torch.randn(4, 5), torch.randn(4, 5))
        model = CustomModule()
        with self.assertRaisesRegex(RuntimeError, 'ExpandedWeights cannot compute the batch size from the inputs'):
            call_for_per_sample_grads(model)(input, '')
        with self.assertRaisesRegex(RuntimeError, 'Expected ExpandedWeights to have batch size matching input'):
            call_for_per_sample_grads(model)(input, torch.randn(5))
        model = CustomModule()
        call_for_per_sample_grads(model)(input, torch.randn(4, 5))
        model = CustomModule()
        call_for_per_sample_grads(model, batch_size=4)(input, torch.randn(5))

class ContextManagerTests(TestBase):

    def __init__(self, *args, **kwargs):
        self.test_cpu = kwargs.get('test_cpu', True)
        self.test_xpu = kwargs.get('test_xpu', True)
        super().__init__(*args, **kwargs)

    @property
    def constructor_args(self):
        return self._get_arg('constructor_args', False)

    def test_context_manager(self, test_case, device):
        kwargs = {'device': device, 'dtype': torch.double}
        module = self.constructor(*self.constructor_args).to(**kwargs)
        if 'Embedding' in self.get_name():
            kwargs['dtype'] = torch.long
        input = self._get_input().to(**kwargs)
        if len(input.shape) == 0 or input.shape[0] == 0:
            raise unittest.SkipTest("Can't get per sample gradients when no batch dim or batch dim is 0")
        if self.constructor == torch.nn.Linear and len(input.shape) == 1:
            raise unittest.SkipTest("Can't get per sample gradients for input of rank 1")
        test_case._do_test(module, input)

    def test_context_manager_multiple_inputs(self, test_case, device):
        module = self.constructor(*self.constructor_args).to(device)
        input = self._get_input()
        if len(input.shape) == 0 or input.shape[0] == 0:
            raise unittest.SkipTest("Can't get per sample gradients when no batch dim or batch dim is 0")
        if self.constructor == torch.nn.Linear and len(input.shape) == 1:
            raise unittest.SkipTest("Can't get per sample gradients for input of rank 1")
        test_case._do_test_multi_input(module, input)

def filter_supported_tests(t):
    supported_modules = ['Linear', 'Conv1d', 'Conv2d', 'Conv3d', 'Embedding', 'LayerNorm', 'GroupNorm', 'InstanceNorm']
    if 'module_name' in t and t['module_name'] in supported_modules:
        return True
supported_tests = [t for t in module_tests + new_module_tests if filter_supported_tests(t)]
for test_param in supported_tests:
    if 'constructor' not in test_param:
        name = test_param.pop('module_name')
        test_param['constructor'] = getattr(nn, name)
    decorator = test_param.pop('decorator', None)
    test = ContextManagerTests(**test_param)
    test_name = test.get_name()
    if hasattr(TestExpandedWeightModule, test_name):
        raise RuntimeError('Found two tests with the same name: ' + test_name)
    test_name_multi_input = test.get_name() + '_multiple_inputs'
    if hasattr(TestExpandedWeightModule, test_name_multi_input):
        raise RuntimeError('Found two tests with the same name: ' + test_name)
    if decorator is not None:
        fn = decorator(fn)
    if test.test_cpu:
        setattr(TestExpandedWeightModule, test_name, lambda self, test=test: test.test_context_manager(self, 'cpu'))
        setattr(TestExpandedWeightModule, test_name_multi_input, lambda self, test=test: test.test_context_manager_multiple_inputs(self, 'cpu'))
    if TEST_XPU and test.test_xpu:
        setattr(TestExpandedWeightModule, test_name + '_xpu_double', lambda self, test=test: test.test_context_manager(self, 'xpu'))

def run_op(op, input, *args, **kwargs):
    """
    OpInfo for Embedding switches the input and weight so autograd tests will only check the derivative
    of the weight, not the input, which can't be differentiable since its dtype is int. Calls op,
    using the special ordering that Embedding's OpInfo expects for that case.
    """
    if op.name == 'nn.functional.embedding':
        return op(args[0], input, **kwargs)
    else:
        return op(input, *args, **kwargs)

def make_expanded_weight(sample_input, batch_size, loss_reduction='sum'):

    def expanded_weight_or_clone(arg):
        if is_diff_tensor(arg):
            return ExpandedWeight(torch.clone(arg), batch_size, loss_reduction)
        return clone_if_tensor(arg)
    ew_input = clone_if_tensor(sample_input.input)
    ew_args = tuple((expanded_weight_or_clone(arg) for arg in sample_input.args))
    ew_kwargs = {name: expanded_weight_or_clone(arg) for (name, arg) in sample_input.kwargs.items()}
    return (ew_input, ew_args, ew_kwargs)

def supported_inputs(op, sample_inputs, supported_inputs=True):
    """
    ExpandedWeights currently does not support some use cases when there's no batch dimension or
    operations that would cause inter-batch operations. Removes all of the cases it cannot deal with
    """

    def filter_fn(input):
        convolutions = ['nn.functional.conv1d', 'nn.functional.conv2d', 'nn.functional.conv3d']
        batched_input_size = dict(zip(convolutions, [3, 4, 5]))
        if op.name == 'nn.functional.linear':
            is_supported_input = input.input.dim() > 1
        elif op.name == 'nn.functional.layer_norm':
            normalized_shape = input.args[0]
            is_supported_input = input.input.shape != normalized_shape
        elif op.name in convolutions:
            is_supported_input = input.input.dim() == batched_input_size[op.name]
        elif op.name == 'nn.functional.embedding':
            idx = input.args[0]
            is_supported_input = len(idx.shape) > 1
        else:
            is_supported_input = True
        is_supported_input = is_supported_input and input.input.shape[0] > 0
        return is_supported_input if supported_inputs else not is_supported_input
    return [input for input in sample_inputs if filter_fn(input)]

def for_loop_per_sample_grad(batch_size, reduction, input, func, *args, **kwargs):
    per_sample_grad = []
    for i in range(batch_size):
        per_sample_input = input[i]
        result = reduction(func(per_sample_input.unsqueeze(0), *args, **kwargs))
        diff_input_list = (per_sample_input,) + tuple(args) + tuple(kwargs.values())
        diff_input_list = [i for i in diff_input_list if isinstance(i, torch.Tensor) and i.requires_grad]
        per_sample_grad.append(torch.autograd.grad(result, diff_input_list, torch.ones_like(result), allow_unused=True))
    if len(per_sample_grad) == batch_size:
        per_sample_grad = tuple((torch.stack(grad) for grad in zip(*per_sample_grad)))
    return per_sample_grad

def is_diff_tensor(t):
    return isinstance(t, ExpandedWeight) or (isinstance(t, torch.Tensor) and t.requires_grad)

def clone_if_tensor(t):
    if isinstance(t, torch.Tensor):
        res = torch.clone(t).detach()
        res.requires_grad_(t.requires_grad)
        return res
    else:
        return t
instantiate_device_type_tests(TestExpandedWeightHelperFunction, globals())
instantiate_device_type_tests(TestExpandedWeightFunctional, globals())
if __name__ == '__main__':
    run_tests()