// Autogenerated file by gen-gpu-ops.py. Do not edit directly!
#include <ATen/aten_ipex_type_default.h>
#include <ATen/aten_ipex_type_dpcpp.h>

#include <ATen/Context.h>
#include <ATen/core/op_registration/op_registration.h>

namespace at {

at::Tensor AtenIpexTypeDefault::add(const at::Tensor & self, const at::Tensor & other, at::Scalar alpha) {
  return AtenIpexTypeDPCPP::add(self, other, alpha);
}

at::Tensor & AtenIpexTypeDefault::add_(at::Tensor & self, const at::Tensor & other, at::Scalar alpha) {
  return AtenIpexTypeDPCPP::add_(self, other, alpha);
}

at::Tensor & AtenIpexTypeDefault::add_out(at::Tensor & out, const at::Tensor & self, const at::Tensor & other, at::Scalar alpha) {
  return AtenIpexTypeDPCPP::add_out(out, self, other, alpha);
}

at::Tensor AtenIpexTypeDefault::add(const at::Tensor & self, at::Scalar other, at::Scalar alpha) {
  return AtenIpexTypeDPCPP::add(self, other, alpha);
}

at::Tensor & AtenIpexTypeDefault::add_(at::Tensor & self, at::Scalar other, at::Scalar alpha) {
  return AtenIpexTypeDPCPP::add_(self, other, alpha);
}

at::Tensor & AtenIpexTypeDefault::arange_out(at::Tensor & out, at::Scalar start, at::Scalar end, at::Scalar step) {
  return AtenIpexTypeDPCPP::arange_out(out, start, end, step);
}

at::Tensor AtenIpexTypeDefault::as_strided(const at::Tensor & self, at::IntArrayRef size, at::IntArrayRef stride, c10::optional<int64_t> storage_offset) {
  return AtenIpexTypeDPCPP::as_strided(self, size, stride, storage_offset);
}

at::Tensor AtenIpexTypeDefault::bitwise_not(const at::Tensor & self) {
  return AtenIpexTypeDPCPP::bitwise_not(self);
}

at::Tensor & AtenIpexTypeDefault::bitwise_not_(at::Tensor & self) {
  return AtenIpexTypeDPCPP::bitwise_not_(self);
}

at::Tensor & AtenIpexTypeDefault::bitwise_not_out(at::Tensor & out, const at::Tensor & self) {
  return AtenIpexTypeDPCPP::bitwise_not_out(out, self);
}

at::Tensor AtenIpexTypeDefault::logical_not(const at::Tensor & self) {
  return AtenIpexTypeDPCPP::logical_not(self);
}

at::Tensor & AtenIpexTypeDefault::logical_not_(at::Tensor & self) {
  return AtenIpexTypeDPCPP::logical_not_(self);
}

at::Tensor & AtenIpexTypeDefault::logical_not_out(at::Tensor & out, const at::Tensor & self) {
  return AtenIpexTypeDPCPP::logical_not_out(out, self);
}

at::Tensor AtenIpexTypeDefault::convolution_overrideable(const at::Tensor & input, const at::Tensor & weight, const at::Tensor & bias, at::IntArrayRef stride, at::IntArrayRef padding, at::IntArrayRef dilation, bool transposed, at::IntArrayRef output_padding, int64_t groups) {
  return AtenIpexTypeDPCPP::convolution_overrideable(input, weight, bias, stride, padding, dilation, transposed, output_padding, groups);
}

at::Tensor & AtenIpexTypeDefault::copy_(at::Tensor & self, const at::Tensor & src, bool non_blocking) {
  return AtenIpexTypeDPCPP::copy_(self, src, non_blocking);
}

at::Tensor AtenIpexTypeDefault::div(const at::Tensor & self, const at::Tensor & other) {
  return AtenIpexTypeDPCPP::div(self, other);
}

at::Tensor & AtenIpexTypeDefault::div_(at::Tensor & self, const at::Tensor & other) {
  return AtenIpexTypeDPCPP::div_(self, other);
}

at::Tensor & AtenIpexTypeDefault::div_out(at::Tensor & out, const at::Tensor & self, const at::Tensor & other) {
  return AtenIpexTypeDPCPP::div_out(out, self, other);
}

at::Tensor AtenIpexTypeDefault::empty(at::IntArrayRef size, const at::TensorOptions & options, c10::optional<at::MemoryFormat> memory_format) {
  return AtenIpexTypeDPCPP::empty(size, options, memory_format);
}

at::Tensor & AtenIpexTypeDefault::resize_(at::Tensor & self, at::IntArrayRef size, c10::optional<at::MemoryFormat> memory_format) {
  return AtenIpexTypeDPCPP::resize_(self, size, memory_format);
}

at::Tensor & AtenIpexTypeDefault::fill_(at::Tensor & self, at::Scalar value) {
  return AtenIpexTypeDPCPP::fill_(self, value);
}

at::Tensor & AtenIpexTypeDefault::fill_(at::Tensor & self, const at::Tensor & value) {
  return AtenIpexTypeDPCPP::fill_(self, value);
}

at::Tensor & AtenIpexTypeDefault::linspace_out(at::Tensor & out, at::Scalar start, at::Scalar end, int64_t steps) {
  return AtenIpexTypeDPCPP::linspace_out(out, start, end, steps);
}

at::Tensor & AtenIpexTypeDefault::logspace_out(at::Tensor & out, at::Scalar start, at::Scalar end, int64_t steps, double base) {
  return AtenIpexTypeDPCPP::logspace_out(out, start, end, steps, base);
}

at::Tensor AtenIpexTypeDefault::_log_softmax(const at::Tensor & self, int64_t dim, bool half_to_float) {
  return AtenIpexTypeDPCPP::_log_softmax(self, dim, half_to_float);
}

at::Tensor AtenIpexTypeDefault::mul(const at::Tensor & self, const at::Tensor & other) {
  return AtenIpexTypeDPCPP::mul(self, other);
}

at::Tensor & AtenIpexTypeDefault::mul_(at::Tensor & self, const at::Tensor & other) {
  return AtenIpexTypeDPCPP::mul_(self, other);
}

at::Tensor & AtenIpexTypeDefault::mul_out(at::Tensor & out, const at::Tensor & self, const at::Tensor & other) {
  return AtenIpexTypeDPCPP::mul_out(out, self, other);
}

at::Tensor AtenIpexTypeDefault::mul(const at::Tensor & self, at::Scalar other) {
  return AtenIpexTypeDPCPP::mul(self, other);
}

at::Tensor & AtenIpexTypeDefault::mul_(at::Tensor & self, at::Scalar other) {
  return AtenIpexTypeDPCPP::mul_(self, other);
}

std::tuple<at::Tensor,at::Tensor,at::Tensor> AtenIpexTypeDefault::native_batch_norm(const at::Tensor & input, const at::Tensor & weight, const at::Tensor & bias, const at::Tensor & running_mean, const at::Tensor & running_var, bool training, double momentum, double eps) {
  return AtenIpexTypeDPCPP::native_batch_norm(input, weight, bias, running_mean, running_var, training, momentum, eps);
}

at::Tensor & AtenIpexTypeDefault::range_out(at::Tensor & out, at::Scalar start, at::Scalar end, at::Scalar step) {
  return AtenIpexTypeDPCPP::range_out(out, start, end, step);
}

at::Tensor AtenIpexTypeDefault::neg(const at::Tensor & self) {
  return AtenIpexTypeDPCPP::neg(self);
}

at::Tensor & AtenIpexTypeDefault::neg_(at::Tensor & self) {
  return AtenIpexTypeDPCPP::neg_(self);
}

at::Tensor & AtenIpexTypeDefault::neg_out(at::Tensor & out, const at::Tensor & self) {
  return AtenIpexTypeDPCPP::neg_out(out, self);
}

at::Tensor & AtenIpexTypeDefault::relu_(at::Tensor & self) {
  return AtenIpexTypeDPCPP::relu_(self);
}

at::Tensor AtenIpexTypeDefault::threshold(const at::Tensor & self, at::Scalar threshold, at::Scalar value) {
  return AtenIpexTypeDPCPP::threshold(self, threshold, value);
}

at::Tensor & AtenIpexTypeDefault::threshold_(at::Tensor & self, at::Scalar threshold, at::Scalar value) {
  return AtenIpexTypeDPCPP::threshold_(self, threshold, value);
}

at::Tensor & AtenIpexTypeDefault::threshold_out(at::Tensor & out, const at::Tensor & self, at::Scalar threshold, at::Scalar value) {
  return AtenIpexTypeDPCPP::threshold_out(out, self, threshold, value);
}

at::Tensor & AtenIpexTypeDefault::resize_as_(at::Tensor & self, const at::Tensor & the_template, c10::optional<at::MemoryFormat> memory_format) {
  return AtenIpexTypeDPCPP::resize_as_(self, the_template, memory_format);
}

at::Tensor & AtenIpexTypeDefault::sub_out(at::Tensor & out, const at::Tensor & self, const at::Tensor & other, at::Scalar alpha) {
  return AtenIpexTypeDPCPP::sub_out(out, self, other, alpha);
}

at::Tensor AtenIpexTypeDefault::sub(const at::Tensor & self, const at::Tensor & other, at::Scalar alpha) {
  return AtenIpexTypeDPCPP::sub(self, other, alpha);
}

at::Tensor & AtenIpexTypeDefault::sub_(at::Tensor & self, const at::Tensor & other, at::Scalar alpha) {
  return AtenIpexTypeDPCPP::sub_(self, other, alpha);
}

at::Tensor AtenIpexTypeDefault::sub(const at::Tensor & self, at::Scalar other, at::Scalar alpha) {
  return AtenIpexTypeDPCPP::sub(self, other, alpha);
}

at::Tensor & AtenIpexTypeDefault::sub_(at::Tensor & self, at::Scalar other, at::Scalar alpha) {
  return AtenIpexTypeDPCPP::sub_(self, other, alpha);
}

at::Tensor AtenIpexTypeDefault::rsub(const at::Tensor & self, const at::Tensor & other, at::Scalar alpha) {
  return AtenIpexTypeDPCPP::rsub(self, other, alpha);
}

at::Tensor AtenIpexTypeDefault::rsub(const at::Tensor & self, at::Scalar other, at::Scalar alpha) {
  return AtenIpexTypeDPCPP::rsub(self, other, alpha);
}

at::Tensor AtenIpexTypeDefault::addmm(const at::Tensor & self, const at::Tensor & mat1, const at::Tensor & mat2, at::Scalar beta, at::Scalar alpha) {
  return AtenIpexTypeDPCPP::addmm(self, mat1, mat2, beta, alpha);
}

at::Tensor AtenIpexTypeDefault::view(const at::Tensor & self, at::IntArrayRef size) {
  return AtenIpexTypeDPCPP::view(self, size);
}

at::Tensor & AtenIpexTypeDefault::tril_(at::Tensor & self, int64_t diagonal) {
  return AtenIpexTypeDPCPP::tril_(self, diagonal);
}

at::Tensor & AtenIpexTypeDefault::triu_(at::Tensor & self, int64_t diagonal) {
  return AtenIpexTypeDPCPP::triu_(self, diagonal);
}

at::Tensor & AtenIpexTypeDefault::triu_out(at::Tensor & out, const at::Tensor & self, int64_t diagonal) {
  return AtenIpexTypeDPCPP::triu_out(out, self, diagonal);
}

at::Tensor & AtenIpexTypeDefault::tril_out(at::Tensor & out, const at::Tensor & self, int64_t diagonal) {
  return AtenIpexTypeDPCPP::tril_out(out, self, diagonal);
}

at::Tensor & AtenIpexTypeDefault::avg_pool2d_out(at::Tensor & out, const at::Tensor & self, at::IntArrayRef kernel_size, at::IntArrayRef stride, at::IntArrayRef padding, bool ceil_mode, bool count_include_pad, c10::optional<int64_t> divisor_override) {
  return AtenIpexTypeDPCPP::avg_pool2d_out(out, self, kernel_size, stride, padding, ceil_mode, count_include_pad, divisor_override);
}

at::Tensor AtenIpexTypeDefault::avg_pool2d(const at::Tensor & self, at::IntArrayRef kernel_size, at::IntArrayRef stride, at::IntArrayRef padding, bool ceil_mode, bool count_include_pad, c10::optional<int64_t> divisor_override) {
  return AtenIpexTypeDPCPP::avg_pool2d(self, kernel_size, stride, padding, ceil_mode, count_include_pad, divisor_override);
}

std::tuple<at::Tensor &,at::Tensor &> AtenIpexTypeDefault::max_pool2d_with_indices_out(at::Tensor & out, at::Tensor & indices, const at::Tensor & self, at::IntArrayRef kernel_size, at::IntArrayRef stride, at::IntArrayRef padding, at::IntArrayRef dilation, bool ceil_mode) {
  return AtenIpexTypeDPCPP::max_pool2d_with_indices_out(out, indices, self, kernel_size, stride, padding, dilation, ceil_mode);
}

std::tuple<at::Tensor,at::Tensor> AtenIpexTypeDefault::max_pool2d_with_indices(const at::Tensor & self, at::IntArrayRef kernel_size, at::IntArrayRef stride, at::IntArrayRef padding, at::IntArrayRef dilation, bool ceil_mode) {
  return AtenIpexTypeDPCPP::max_pool2d_with_indices(self, kernel_size, stride, padding, dilation, ceil_mode);
}



void RegisterAtenTypeFunctions() {
  static auto dispatch = torch::RegisterOperators()
  .op(torch::RegisterOperators::options().schema("aten::add.Tensor(Tensor self, Tensor other, *, Scalar alpha=1) -> Tensor")
      .impl_unboxedOnlyKernel<at::Tensor(const at::Tensor &, const at::Tensor &, at::Scalar), &AtenIpexTypeDefault::add>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::add_.Tensor(Tensor(a!) self, Tensor other, *, Scalar alpha=1) -> Tensor(a!)")
      .impl_unboxedOnlyKernel<at::Tensor &(at::Tensor &, const at::Tensor &, at::Scalar), &AtenIpexTypeDefault::add_>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::add.out(Tensor self, Tensor other, *, Scalar alpha=1, Tensor(a!) out) -> Tensor(a!)")
      .impl_unboxedOnlyKernel<at::Tensor &(at::Tensor &, const at::Tensor &, const at::Tensor &, at::Scalar), &AtenIpexTypeDefault::add_out>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::add.Scalar(Tensor self, Scalar other, Scalar alpha=1) -> Tensor")
      .impl_unboxedOnlyKernel<at::Tensor(const at::Tensor &, at::Scalar, at::Scalar), &AtenIpexTypeDefault::add>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::add_.Scalar(Tensor(a!) self, Scalar other, Scalar alpha=1) -> Tensor(a!)")
      .impl_unboxedOnlyKernel<at::Tensor &(at::Tensor &, at::Scalar, at::Scalar), &AtenIpexTypeDefault::add_>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::arange.start_out(Scalar start, Scalar end, Scalar step=1, *, Tensor(a!) out) -> Tensor(a!)")
      .impl_unboxedOnlyKernel<at::Tensor &(at::Tensor &, at::Scalar, at::Scalar, at::Scalar), &AtenIpexTypeDefault::arange_out>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::as_strided(Tensor(a) self, int[] size, int[] stride, int? storage_offset=None) -> Tensor(a)")
      .impl_unboxedOnlyKernel<at::Tensor(const at::Tensor &, at::IntArrayRef, at::IntArrayRef, c10::optional<int64_t>), &AtenIpexTypeDefault::as_strided>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::bitwise_not(Tensor self) -> Tensor")
      .impl_unboxedOnlyKernel<at::Tensor(const at::Tensor &), &AtenIpexTypeDefault::bitwise_not>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::bitwise_not_(Tensor(a!) self) -> Tensor(a!)")
      .impl_unboxedOnlyKernel<at::Tensor &(at::Tensor &), &AtenIpexTypeDefault::bitwise_not_>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::bitwise_not.out(Tensor self, *, Tensor(a!) out) -> Tensor(a!)")
      .impl_unboxedOnlyKernel<at::Tensor &(at::Tensor &, const at::Tensor &), &AtenIpexTypeDefault::bitwise_not_out>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::logical_not(Tensor self) -> Tensor")
      .impl_unboxedOnlyKernel<at::Tensor(const at::Tensor &), &AtenIpexTypeDefault::logical_not>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::logical_not_(Tensor(a!) self) -> Tensor(a!)")
      .impl_unboxedOnlyKernel<at::Tensor &(at::Tensor &), &AtenIpexTypeDefault::logical_not_>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::logical_not.out(Tensor self, *, Tensor(a!) out) -> Tensor(a!)")
      .impl_unboxedOnlyKernel<at::Tensor &(at::Tensor &, const at::Tensor &), &AtenIpexTypeDefault::logical_not_out>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::convolution_overrideable(Tensor input, Tensor weight, Tensor? bias, int[] stride, int[] padding, int[] dilation, bool transposed, int[] output_padding, int groups) -> Tensor")
      .impl_unboxedOnlyKernel<at::Tensor(const at::Tensor &, const at::Tensor &, const at::Tensor &, at::IntArrayRef, at::IntArrayRef, at::IntArrayRef, bool, at::IntArrayRef, int64_t), &AtenIpexTypeDefault::convolution_overrideable>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::copy_(Tensor(a!) self, Tensor src, bool non_blocking=False) -> Tensor(a!)")
      .impl_unboxedOnlyKernel<at::Tensor &(at::Tensor &, const at::Tensor &, bool), &AtenIpexTypeDefault::copy_>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::div.Tensor(Tensor self, Tensor other) -> Tensor")
      .impl_unboxedOnlyKernel<at::Tensor(const at::Tensor &, const at::Tensor &), &AtenIpexTypeDefault::div>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::div_.Tensor(Tensor(a!) self, Tensor other) -> Tensor(a!)")
      .impl_unboxedOnlyKernel<at::Tensor &(at::Tensor &, const at::Tensor &), &AtenIpexTypeDefault::div_>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::div.out(Tensor self, Tensor other, *, Tensor(a!) out) -> Tensor(a!)")
      .impl_unboxedOnlyKernel<at::Tensor &(at::Tensor &, const at::Tensor &, const at::Tensor &), &AtenIpexTypeDefault::div_out>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::empty.memory_format(int[] size, *, ScalarType? dtype=None, Layout? layout=None, Device? device=None, bool? pin_memory=None, MemoryFormat? memory_format=None) -> Tensor")
      .impl_unboxedOnlyKernel<at::Tensor(at::IntArrayRef, const at::TensorOptions &, c10::optional<at::MemoryFormat>), &AtenIpexTypeDefault::empty>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::resize_(Tensor(a!) self, int[] size, *, MemoryFormat? memory_format=None) -> Tensor(a!)")
      .impl_unboxedOnlyKernel<at::Tensor &(at::Tensor &, at::IntArrayRef, c10::optional<at::MemoryFormat>), &AtenIpexTypeDefault::resize_>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::fill_.Scalar(Tensor(a!) self, Scalar value) -> Tensor(a!)")
      .impl_unboxedOnlyKernel<at::Tensor &(at::Tensor &, at::Scalar), &AtenIpexTypeDefault::fill_>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::fill_.Tensor(Tensor(a!) self, Tensor value) -> Tensor(a!)")
      .impl_unboxedOnlyKernel<at::Tensor &(at::Tensor &, const at::Tensor &), &AtenIpexTypeDefault::fill_>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::linspace.out(Scalar start, Scalar end, int steps=100, *, Tensor(a!) out) -> Tensor(a!)")
      .impl_unboxedOnlyKernel<at::Tensor &(at::Tensor &, at::Scalar, at::Scalar, int64_t), &AtenIpexTypeDefault::linspace_out>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::logspace.out(Scalar start, Scalar end, int steps=100, float base=10.0, *, Tensor(a!) out) -> Tensor(a!)")
      .impl_unboxedOnlyKernel<at::Tensor &(at::Tensor &, at::Scalar, at::Scalar, int64_t, double), &AtenIpexTypeDefault::logspace_out>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::_log_softmax(Tensor self, int dim, bool half_to_float) -> Tensor")
      .impl_unboxedOnlyKernel<at::Tensor(const at::Tensor &, int64_t, bool), &AtenIpexTypeDefault::_log_softmax>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::mul.Tensor(Tensor self, Tensor other) -> Tensor")
      .impl_unboxedOnlyKernel<at::Tensor(const at::Tensor &, const at::Tensor &), &AtenIpexTypeDefault::mul>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::mul_.Tensor(Tensor(a!) self, Tensor other) -> Tensor(a!)")
      .impl_unboxedOnlyKernel<at::Tensor &(at::Tensor &, const at::Tensor &), &AtenIpexTypeDefault::mul_>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::mul.out(Tensor self, Tensor other, *, Tensor(a!) out) -> Tensor(a!)")
      .impl_unboxedOnlyKernel<at::Tensor &(at::Tensor &, const at::Tensor &, const at::Tensor &), &AtenIpexTypeDefault::mul_out>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::mul.Scalar(Tensor self, Scalar other) -> Tensor")
      .impl_unboxedOnlyKernel<at::Tensor(const at::Tensor &, at::Scalar), &AtenIpexTypeDefault::mul>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::mul_.Scalar(Tensor(a!) self, Scalar other) -> Tensor(a!)")
      .impl_unboxedOnlyKernel<at::Tensor &(at::Tensor &, at::Scalar), &AtenIpexTypeDefault::mul_>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::native_batch_norm(Tensor input, Tensor? weight, Tensor? bias, Tensor? running_mean, Tensor? running_var, bool training, float momentum, float eps) -> (Tensor, Tensor, Tensor)")
      .impl_unboxedOnlyKernel<std::tuple<at::Tensor,at::Tensor,at::Tensor>(const at::Tensor &, const at::Tensor &, const at::Tensor &, const at::Tensor &, const at::Tensor &, bool, double, double), &AtenIpexTypeDefault::native_batch_norm>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::range.out(Scalar start, Scalar end, Scalar step=1, *, Tensor(a!) out) -> Tensor(a!)")
      .impl_unboxedOnlyKernel<at::Tensor &(at::Tensor &, at::Scalar, at::Scalar, at::Scalar), &AtenIpexTypeDefault::range_out>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::neg(Tensor self) -> Tensor")
      .impl_unboxedOnlyKernel<at::Tensor(const at::Tensor &), &AtenIpexTypeDefault::neg>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::neg_(Tensor(a!) self) -> Tensor(a!)")
      .impl_unboxedOnlyKernel<at::Tensor &(at::Tensor &), &AtenIpexTypeDefault::neg_>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::neg.out(Tensor self, *, Tensor(a!) out) -> Tensor(a!)")
      .impl_unboxedOnlyKernel<at::Tensor &(at::Tensor &, const at::Tensor &), &AtenIpexTypeDefault::neg_out>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::relu_(Tensor(a!) self) -> Tensor(a!)")
      .impl_unboxedOnlyKernel<at::Tensor &(at::Tensor &), &AtenIpexTypeDefault::relu_>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::threshold(Tensor self, Scalar threshold, Scalar value) -> Tensor")
      .impl_unboxedOnlyKernel<at::Tensor(const at::Tensor &, at::Scalar, at::Scalar), &AtenIpexTypeDefault::threshold>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::threshold_(Tensor(a!) self, Scalar threshold, Scalar value) -> Tensor(a!)")
      .impl_unboxedOnlyKernel<at::Tensor &(at::Tensor &, at::Scalar, at::Scalar), &AtenIpexTypeDefault::threshold_>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::threshold.out(Tensor self, Scalar threshold, Scalar value, *, Tensor(a!) out) -> Tensor(a!)")
      .impl_unboxedOnlyKernel<at::Tensor &(at::Tensor &, const at::Tensor &, at::Scalar, at::Scalar), &AtenIpexTypeDefault::threshold_out>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::resize_as_(Tensor(a!) self, Tensor the_template, *, int? memory_format=None) -> (Tensor(a!))")
      .impl_unboxedOnlyKernel<at::Tensor &(at::Tensor &, const at::Tensor &, c10::optional<at::MemoryFormat>), &AtenIpexTypeDefault::resize_as_>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::sub.out(Tensor self, Tensor other, *, Scalar alpha=1, Tensor(a!) out) -> Tensor(a!)")
      .impl_unboxedOnlyKernel<at::Tensor &(at::Tensor &, const at::Tensor &, const at::Tensor &, at::Scalar), &AtenIpexTypeDefault::sub_out>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::sub.Tensor(Tensor self, Tensor other, *, Scalar alpha=1) -> Tensor")
      .impl_unboxedOnlyKernel<at::Tensor(const at::Tensor &, const at::Tensor &, at::Scalar), &AtenIpexTypeDefault::sub>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::sub_.Tensor(Tensor(a!) self, Tensor other, *, Scalar alpha=1) -> Tensor(a!)")
      .impl_unboxedOnlyKernel<at::Tensor &(at::Tensor &, const at::Tensor &, at::Scalar), &AtenIpexTypeDefault::sub_>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::sub.Scalar(Tensor self, Scalar other, Scalar alpha=1) -> Tensor")
      .impl_unboxedOnlyKernel<at::Tensor(const at::Tensor &, at::Scalar, at::Scalar), &AtenIpexTypeDefault::sub>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::sub_.Scalar(Tensor(a!) self, Scalar other, Scalar alpha=1) -> Tensor(a!)")
      .impl_unboxedOnlyKernel<at::Tensor &(at::Tensor &, at::Scalar, at::Scalar), &AtenIpexTypeDefault::sub_>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::rsub.Tensor(Tensor self, Tensor other, *, Scalar alpha=1) -> Tensor")
      .impl_unboxedOnlyKernel<at::Tensor(const at::Tensor &, const at::Tensor &, at::Scalar), &AtenIpexTypeDefault::rsub>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::rsub.Scalar(Tensor self, Scalar other, Scalar alpha=1) -> Tensor")
      .impl_unboxedOnlyKernel<at::Tensor(const at::Tensor &, at::Scalar, at::Scalar), &AtenIpexTypeDefault::rsub>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::addmm(Tensor self, Tensor mat1, Tensor mat2, *, Scalar beta=1, Scalar alpha=1) -> Tensor")
      .impl_unboxedOnlyKernel<at::Tensor(const at::Tensor &, const at::Tensor &, const at::Tensor &, at::Scalar, at::Scalar), &AtenIpexTypeDefault::addmm>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::view(Tensor(a) self, int[] size) -> Tensor(a)")
      .impl_unboxedOnlyKernel<at::Tensor(const at::Tensor &, at::IntArrayRef), &AtenIpexTypeDefault::view>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::tril_(Tensor(a!) self, int diagonal=0) -> Tensor(a!)")
      .impl_unboxedOnlyKernel<at::Tensor &(at::Tensor &, int64_t), &AtenIpexTypeDefault::tril_>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::triu_(Tensor(a!) self, int diagonal=0) -> Tensor(a!)")
      .impl_unboxedOnlyKernel<at::Tensor &(at::Tensor &, int64_t), &AtenIpexTypeDefault::triu_>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::triu.out(Tensor self, int diagonal=0, *, Tensor(a!) out) -> Tensor(a!)")
      .impl_unboxedOnlyKernel<at::Tensor &(at::Tensor &, const at::Tensor &, int64_t), &AtenIpexTypeDefault::triu_out>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::tril.out(Tensor self, int diagonal=0, *, Tensor(a!) out) -> Tensor(a!)")
      .impl_unboxedOnlyKernel<at::Tensor &(at::Tensor &, const at::Tensor &, int64_t), &AtenIpexTypeDefault::tril_out>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::avg_pool2d.out(Tensor self, int[2] kernel_size, int[2] stride=[], int[2] padding=0, bool ceil_mode=False, bool count_include_pad=True, int? divisor_override=None, *, Tensor(a!) out) -> Tensor(a!)")
      .impl_unboxedOnlyKernel<at::Tensor &(at::Tensor &, const at::Tensor &, at::IntArrayRef, at::IntArrayRef, at::IntArrayRef, bool, bool, c10::optional<int64_t>), &AtenIpexTypeDefault::avg_pool2d_out>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::avg_pool2d(Tensor self, int[2] kernel_size, int[2] stride=[], int[2] padding=0, bool ceil_mode=False, bool count_include_pad=True, int? divisor_override=None) -> Tensor")
      .impl_unboxedOnlyKernel<at::Tensor(const at::Tensor &, at::IntArrayRef, at::IntArrayRef, at::IntArrayRef, bool, bool, c10::optional<int64_t>), &AtenIpexTypeDefault::avg_pool2d>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::max_pool2d_with_indices.out(Tensor self, int[2] kernel_size, int[2] stride=[], int[2] padding=0, int[2] dilation=1, bool ceil_mode=False, *, Tensor(a!) out, Tensor(b!) indices) -> (Tensor(a!), Tensor(b!))")
      .impl_unboxedOnlyKernel<std::tuple<at::Tensor &,at::Tensor &>(at::Tensor &, at::Tensor &, const at::Tensor &, at::IntArrayRef, at::IntArrayRef, at::IntArrayRef, at::IntArrayRef, bool), &AtenIpexTypeDefault::max_pool2d_with_indices_out>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
  .op(torch::RegisterOperators::options().schema("aten::max_pool2d_with_indices(Tensor self, int[2] kernel_size, int[2] stride=[], int[2] padding=0, int[2] dilation=1, bool ceil_mode=False) -> (Tensor, Tensor)")
      .impl_unboxedOnlyKernel<std::tuple<at::Tensor,at::Tensor>(const at::Tensor &, at::IntArrayRef, at::IntArrayRef, at::IntArrayRef, at::IntArrayRef, bool), &AtenIpexTypeDefault::max_pool2d_with_indices>(at::TensorTypeId::DPCPPTensorId)
      .aliasAnalysis(c10::AliasAnalysisKind::FROM_SCHEMA))
;
}

}  // namespace at

