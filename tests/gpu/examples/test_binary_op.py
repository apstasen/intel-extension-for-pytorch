import torch
import torch.nn as nn
from torch.testing._internal.common_utils import TestCase
import torch_ipex

cpu_device = torch.device("cpu")
dpcpp_device = torch.device("xpu")


class TestTorchMethod(TestCase):
    def test_binary_op_channels_last(self, dtype=torch.float):
        a_cpu = torch.randn(1,2,3,4)
        b_cpu = torch.randn(1,2,3,4)
        y_cpu = a_cpu + b_cpu

        a_xpu = a_cpu.to("xpu").to(memory_format=torch.channels_last)
        b_xpu = b_cpu.to("xpu").to(memory_format=torch.channels_last)
        # a_xpu = a_cpu.to("xpu")
        # b_xpu = b_cpu.to("xpu")
        y_xpu = a_xpu + b_xpu
        self.assertEqual(y_cpu, y_xpu.cpu())

    def test_binary_op(self, dtype=torch.float):
        x_cpu = torch.randn(5)

        x_dpcpp = x_cpu.to(dpcpp_device)
        #y_cpu1 = x_cpu.new_ones((2, 3))
        y_cpu1 = torch.randn(5)
        #y_cpu2 = x_cpu.new_ones((2, 3))
        y_cpu2 = torch.randn(5)

        y_cpu1_int = torch.tensor(
            [[3, 1, 2, 3], [2, 3, 4, 1]], dtype=torch.int32)
        #y_cpu2 = x_cpu.new_ones((2, 3))
        y_cpu2_int = torch.tensor(
            [[1, 5, 2, 4], [1, 1, 5, 5]], dtype=torch.int32)

        y_dpcpp1 = y_cpu1.to(dpcpp_device)
        y_dpcpp2 = y_cpu2.to(dpcpp_device)
        y_dpcpp1_int = y_cpu1_int.to(dpcpp_device)
        y_dpcpp2_int = y_cpu2_int.to(dpcpp_device)

        x_cpu_b_1 = torch.tensor([True, True])
        x_cpu_b_2 = torch.tensor([False, True])
        x_dpcpp_b_1 = x_cpu_b_1.to(dpcpp_device)
        x_dpcpp_b_2 = x_cpu_b_2.to(dpcpp_device)

        print("add y_cpu", y_cpu1.add(y_cpu2))
        print("add y_dpcpp", y_dpcpp1.add(y_dpcpp2).to(cpu_device))
        self.assertEqual(y_cpu1.add(y_cpu2),
                         y_dpcpp1.add(y_dpcpp2).to(cpu_device))

        print("sub y_cpu", y_cpu1.sub(y_cpu2))
        print("sub y_dpcpp", y_dpcpp1.sub(y_dpcpp2).to(cpu_device))
        self.assertEqual(y_cpu1.sub(y_cpu2),
                         y_dpcpp1.sub(y_dpcpp2).to(cpu_device))

        print("mul y_cpu", y_cpu1.mul(y_cpu2))
        print("mul y_dpcpp", y_dpcpp1.mul(y_dpcpp2).to(cpu_device))
        self.assertEqual(y_cpu1.mul(y_cpu2),
                         y_dpcpp1.mul(y_dpcpp2).to(cpu_device))

        print("div y_cpu", y_cpu1.div(y_cpu2))
        print("div y_dpcpp", y_dpcpp1.div(y_dpcpp2).to(cpu_device))
        self.assertEqual(y_cpu1.div(y_cpu2),
                         y_dpcpp1.div(y_dpcpp2).to(cpu_device))

        print("floor_divide y_cpu", y_cpu1.floor_divide(y_cpu2))
        print("floor_divide y_dpcpp", y_dpcpp1.floor_divide(y_dpcpp2).to(cpu_device))
        self.assertEqual(y_cpu1.floor_divide(y_cpu2),
                         (y_dpcpp1.floor_divide(y_dpcpp2)).to(cpu_device))

        print("__and__ y_cpu", y_cpu1_int.__and__(y_cpu2_int))
        print("__and__ y_dpcpp", y_dpcpp1_int.__and__(
            y_dpcpp2_int).to(cpu_device))
        self.assertEqual(y_cpu1_int.__and__(y_cpu2_int),
                         y_dpcpp1_int.__and__(y_dpcpp2_int).to(cpu_device))

        print("__and__ y_cpu", x_cpu_b_1.__and__(x_cpu_b_2))
        print("__and__ y_dpcpp", x_dpcpp_b_1.__and__(x_dpcpp_b_2).to(cpu_device))
        self.assertEqual(x_cpu_b_1.__and__(x_cpu_b_2),
                         x_dpcpp_b_1.__and__(x_dpcpp_b_2).to(cpu_device))

        print("__iand__ y_cpu", y_cpu1_int.__iand__(y_cpu2_int))
        print("__iand__ y_dpcpp", y_dpcpp1_int.__iand__(
            y_dpcpp2_int).to(cpu_device))
        self.assertEqual(y_cpu1_int.__iand__(y_cpu2_int),
                         y_dpcpp1_int.__iand__(y_dpcpp2_int).to(cpu_device))

        print("__iand__ y_cpu", x_cpu_b_1.__iand__(x_cpu_b_2))
        print("__iand__ y_dpcpp", x_dpcpp_b_1.__iand__(
            x_dpcpp_b_2).to(cpu_device))
        self.assertEqual(x_cpu_b_1.__iand__(x_cpu_b_2),
                         x_dpcpp_b_1.__iand__(x_dpcpp_b_2).to(cpu_device))

        print("__or__ y_cpu", y_cpu1_int.__or__(y_cpu2_int))
        print("__or__ y_dpcpp", y_dpcpp1_int.__or__(
            y_dpcpp2_int).to(cpu_device))
        self.assertEqual(y_cpu1_int.__or__(y_cpu2_int),
                         y_dpcpp1_int.__or__(y_dpcpp2_int).to(cpu_device))

        print("__or__ y_cpu", x_cpu_b_1.__or__(x_cpu_b_2))
        print("__or__ y_dpcpp", x_dpcpp_b_1.__or__(x_dpcpp_b_2).to(cpu_device))
        self.assertEqual(x_cpu_b_1.__or__(x_cpu_b_2),
                         x_dpcpp_b_1.__or__(x_dpcpp_b_2).to(cpu_device))

        print("__ior__ y_cpu", y_cpu1_int.__ior__(y_cpu2_int))
        print("__ior__ y_dpcpp", y_dpcpp1_int.__ior__(
            y_dpcpp2_int).to(cpu_device))
        self.assertEqual(y_cpu1_int.__ior__(y_cpu2_int),
                         y_dpcpp1_int.__ior__(y_dpcpp2_int).to(cpu_device))

        print("__ior__ y_cpu", x_cpu_b_1.__ior__(x_cpu_b_2))
        print("__ior__ y_dpcpp", x_dpcpp_b_1.__ior__(x_dpcpp_b_2).to(cpu_device))
        self.assertEqual(x_cpu_b_1.__ior__(x_cpu_b_2),
                         x_dpcpp_b_1.__ior__(x_dpcpp_b_2).to(cpu_device))

        print("__xor__ y_cpu", y_cpu1_int.__xor__(y_cpu2_int))
        print("__xor__ y_dpcpp", y_dpcpp1_int.__xor__(
            y_dpcpp2_int).to(cpu_device))
        self.assertEqual(y_cpu1_int.__xor__(y_cpu2_int),
                         y_dpcpp1_int.__xor__(y_dpcpp2_int).to(cpu_device))

        print("__xor__ x_cpu", x_cpu_b_1.__xor__(x_cpu_b_2))
        print("__xor__ x_dpcpp", x_dpcpp_b_1.__xor__(x_dpcpp_b_2).to(cpu_device))
        self.assertEqual(x_cpu_b_1.__xor__(x_cpu_b_2),
                         x_dpcpp_b_1.__xor__(x_dpcpp_b_2).to(cpu_device))

        print("remainder scalar y_cpu", torch.remainder(y_cpu1, 1.5))
        print("remainder scalar y_dpcpp", torch.remainder(
            y_dpcpp1, 1.5).to(cpu_device))
        self.assertEqual(torch.remainder(y_cpu1, 1.5),
                         torch.remainder(y_dpcpp1, 1.5).to(cpu_device))

        print("remainder tensor y_cpu", torch.remainder(y_cpu1, y_cpu2))
        print("remainder tensor y_dpcpp", torch.remainder(
            y_dpcpp1, y_dpcpp2).to(cpu_device))
        self.assertEqual(torch.remainder(y_cpu1, y_cpu2),
                         torch.remainder(y_dpcpp1, y_dpcpp2).to(cpu_device))

        print("fmod scalar y_cpu", torch.fmod(y_cpu1, 1.5))
        print("fmod scalar y_dpcpp", torch.fmod(y_dpcpp1, 1.5).to(cpu_device))
        self.assertEqual(torch.fmod(y_cpu1, 1.5),
                         torch.fmod(y_dpcpp1, 1.5).to(cpu_device))

        print("fmod tensor y_cpu", torch.fmod(y_cpu1, y_cpu2))
        print("fmod tensor y_dpcpp", torch.fmod(
            y_dpcpp1, y_dpcpp2).to(cpu_device))
        self.assertEqual(torch.fmod(y_cpu1, y_cpu2), torch.fmod(
            y_dpcpp1, y_dpcpp2).to(cpu_device))

    def test_add_with_alpha_block_format(self, dtype=torch.float):
        x1 = torch.randn(1, 2, 3, 3)
        x1_xpu = x1.to("xpu")
        x2 = torch.randn(1, 2, 3, 3)
        x2_xpu = x2.to("xpu")
        conv = nn.Conv2d(2, 2, kernel_size=3, stride=1, padding=1, bias=False)

        y = conv(x1)
        y.add_(x2, alpha=10)
        x1.add_(y, alpha=0.1)
        print(x1)

        conv.to("xpu")
        y_xpu = conv(x1_xpu)
        y_xpu.add_(x2_xpu, alpha=10)
        x1_xpu.add_(y_xpu, alpha=0.1)
        print(x1_xpu.cpu())

        self.assertEqual(x1, x1_xpu.cpu())

        a = torch.tensor([True, False], device="xpu")
        b = torch.tensor([False, True], device="xpu")
        t = torch.tensor([True, True], device="xpu")

        assert torch.equal(a + b, t)
