#ifndef THSYCL_TENSORMATH_REDUCE_H
#define THSYCL_TENSORMATH_REDUCE_H

#include <core/SYCL.h>
#include <core/SYCLMemory.h>
#include <core/SYCLUtils.h>
#include <legacy/THSYCLNumerics.h>
#include <legacy/THSYCLTensor.h>
#include <legacy/THSYCLGeneral.h>
#include <legacy/generic/THSYCLTensorMathReduce.h>
#include <legacy/THSYCLGenerateAllTypes.h>
#include <legacy/THSYCLTensorCopy.hpp>
#include <legacy/THSYCLTensorMath.h>
#include <legacy/THSYCLAlgorithm.h>
#include <legacy/THSYCLTensorMathReduce.h>

template <typename scalar_t>
DP_DEVICE struct AddOp {
  scalar_t operator() (const scalar_t& lhs, const scalar_t& rhs) const {
    return lhs + rhs;
  }
};

template <typename scalar_t>
DP_DEVICE struct MulOp {
  scalar_t operator() (const scalar_t& lhs, const scalar_t& rhs) const {
    return lhs * rhs;
  }
};

DP_DEVICE struct LogicalAll {
  unsigned char operator()(unsigned char const x, unsigned char const y) const {
    return (x && y);
  }
};

DP_DEVICE struct LogicalAny {
  unsigned char operator()(unsigned char const x, unsigned char const y) const {
    return (x || y);
  }
};

template <typename T>
DP_DEVICE struct ReduceAdd {
  T operator()(const T a, const T b) const {
    return THSYCLNumerics<T>::add(a, b);
  }
};

template <typename T>
DP_DEVICE struct ReduceMin {
  T operator()(T a, T b) const {
    return (THSYCLNumerics<T>::lt(a, b) || THSYCLNumerics<T>::isnan(a)) ? a : b;
  }
};

template <typename T>
DP_DEVICE struct ReduceMax {
  T operator()(T a, T b) const {
    return (THSYCLNumerics<T>::gt(a, b) || THSYCLNumerics<T>::isnan(a)) ? a : b;
  }
};

DP_DEF_K1(reduceInnermostDimIndex);

DP_DEF_K1(reduceOuterDimIndex);

template <typename K,
          typename Index,
          typename TensorTypeK,
          typename TensorTypeIndex,
          class BinaryFunction>
DP_DEVICE void
kernelTransformReduceInnermostDimIndex(K *tgtK,
                                       Index *tgtI,
                                       K *srcK,
                                       TensorTypeK *tgt1,
                                       TensorTypeIndex *tgt2,
                                       TensorTypeK *src,
                                       std::pair<K, Index> init,
                                       BinaryFunction binary_op) {

  auto queue         = c10::sycl::syclGetCurrentQueue();
  int64_t group_size = c10::sycl::syclMaxWorkGroupSize(queue);
  auto totalElements = src->numel();
  auto num_groups    = THSYCLCeilDiv(totalElements, group_size);
  auto total_items   = num_groups * group_size;

  auto tgt1_data     = tgt1->data();
  auto tgt2_data     = tgt2->data();
  auto tgt1_size     = (tgt1->numel()) * (tgt1->dtype().itemsize());
  auto tgt2_size     = (tgt2->numel()) * (tgt2->dtype().itemsize());
  auto src_data      = src->data();
  auto src_size      = totalElements * (src->dtype().itemsize());
  auto dim           = tgt1->dim() - 1;

  int64_t n = src->size(dim);
  int64_t stride = src->stride(dim);
  int64_t batch = totalElements / (n * stride);

  auto cgf = DP_Q_CGF(cgh) {
    auto src_acc  = c10::sycl::SYCLAccessor<dp_r_mode>(cgh, src_data, src_size);
    auto tgt1_acc = c10::sycl::SYCLAccessor<dp_w_mode>(cgh, tgt1_data, tgt1_size);
    auto tgt2_acc = c10::sycl::SYCLAccessor<dp_w_mode>(cgh, tgt2_data, tgt2_size);

    auto kfn = DP_Q_KFN(DP::nd_item<1>item) {
      auto src_ptr  = src_acc.template get_pointer<K>();
      auto tgt1_ptr = tgt1_acc.template get_pointer<K>();
      auto tgt2_ptr = tgt2_acc.template get_pointer<Index>();

      for (int64_t linearIndex = item.get_global_id(0);
           linearIndex < totalElements; linearIndex += item.get_global_range()[0]) {
        int64_t base_start = linearIndex % (batch * stride);
        int64_t start = linearIndex % batch * n;

        std::pair<K, Index> acc = init;
        for (int64_t j = 0; j < n; ++j) {
          //
          // The explicit typecast looks weired but is necessory to solve the build error
          // "candidate function not viable: no known conversion from '__global long' to
          // '__global long &&' for 1st argument."
          //
          K data = src_ptr[start + j];
          Index idx = j /* + TH_INDEX_BASE */;
          acc = binary_op (acc, std::make_pair<K, Index>((K)data, (Index)idx));
      }
        tgt1_ptr[base_start] = acc.first;
        tgt2_ptr[base_start] = acc.second;
      }
    };

    // kick off kernel
    cgh.parallel_for<DP_K(reduceInnermostDimIndex, K, Index, BinaryFunction)>(
      DP::nd_range<1>(DP::range<1>(total_items), DP::range<1>(group_size)), kfn);
  };

  // submit to SYCL queue
  DP_Q_ASYNC_SUBMIT(queue, cgf);
}

template <typename K,
          typename Index,
          typename TensorTypeK,
          typename TensorTypeIndex,
          class BinaryFunction>
DP_DEVICE void
kernelTransformReduceOuterDimIndex(K *tgtK,
                                   Index *tgtI,
                                   K *srcK,
                                   TensorTypeK *tgt1,
                                   TensorTypeIndex *tgt2,
                                   TensorTypeK *src,
                                   int64_t rdim,
                                   std::pair<K, Index> init,
                                   BinaryFunction binary_op) {

  auto queue         = c10::sycl::syclGetCurrentQueue();
  int64_t group_size = c10::sycl::syclMaxWorkGroupSize(queue);
  auto totalElements = src->numel();
  auto num_groups    = THSYCLCeilDiv(totalElements, group_size);
  auto total_items   = num_groups * group_size;

  auto tgt1_data     = tgt1->data();
  auto tgt2_data     = tgt2->data();
  auto tgt1_size     = (tgt1->numel()) * (tgt1->dtype().itemsize());
  auto tgt2_size     = (tgt2->numel()) * (tgt2->dtype().itemsize());
  auto src_data      = src->data();
  auto src_size      = totalElements * (src->dtype().itemsize());

  int64_t n = src->size(rdim);
  int64_t stride = src->stride(rdim);
  int64_t batch = totalElements / (n * stride);

  auto cgf = DP_Q_CGF(cgh) {
    auto src_acc  = c10::sycl::SYCLAccessor<dp_r_mode>(cgh, src_data, src_size);
    auto tgt1_acc = c10::sycl::SYCLAccessor<dp_w_mode>(cgh, tgt1_data, tgt1_size);
    auto tgt2_acc = c10::sycl::SYCLAccessor<dp_w_mode>(cgh, tgt2_data, tgt2_size);

    auto kfn = DP_Q_KFN(DP::nd_item<1>item) {
      auto src_ptr  = src_acc.template get_pointer<K>();
      auto tgt1_ptr = tgt1_acc.template get_pointer<K>();
      auto tgt2_ptr = tgt2_acc.template get_pointer<Index>();

      for (int64_t linearIndex = item.get_global_id(0);
           linearIndex < totalElements; linearIndex += item.get_global_range()[0]) {
        int64_t base_start = linearIndex % (batch * stride);
        int64_t start = (base_start / stride) * n * stride + base_start % stride;

        std::pair<K, Index> acc = init;
        for (int64_t j = 0; j < n; ++j) {
          //
          // The explicit typecast looks weired but is necessory to solve the build error
          // "candidate function not viable: no known conversion from '__global long' to
          // '__global long &&' for 1st argument."
          //
          K data = src_ptr[start + j * stride];
          Index idx = j /* + TH_INDEX_BASE */;
          acc = binary_op (acc, std::make_pair<K, Index>((K)data, (Index)idx));
          tgt1_ptr[base_start] = acc.first;
          tgt2_ptr[base_start] = acc.second;
        }
      }
    };

    // kick off kernel
    cgh.parallel_for<DP_K(reduceOuterDimIndex, K, Index, BinaryFunction)>(
      DP::nd_range<1>(DP::range<1>(total_items), DP::range<1>(group_size)), kfn);
  };

  // submit to SYCL queue
  DP_Q_ASYNC_SUBMIT(queue, cgf);
};

template <typename ScalarTypeK,
          typename ScalarTypeIndex,
          typename TensorTypeK,
          typename TensorTypeIndex,
          typename BinaryFunction>
DP_HOST void
THSYCL_transformReduceOuterDimIndex(THSYCLState *state,
                                    TensorTypeK *tgt1,
                                    TensorTypeIndex *tgt2,
                                    TensorTypeK *src,
                                    int64_t rdim,
                                    const std::pair<ScalarTypeK, ScalarTypeIndex>& init,
                                    BinaryFunction binary_op) {

  kernelTransformReduceOuterDimIndex (
    tgt1->template data<ScalarTypeK>(),
    tgt2->template data<ScalarTypeIndex>(),
    src->template data<ScalarTypeK>(),
    tgt1, tgt2, src, rdim, init, binary_op);
}

template <typename ScalarTypeK,
          typename ScalarTypeIndex,
          typename TensorTypeK,
          typename TensorTypeIndex,
          typename BinaryFunction>
DP_HOST void
THSYCL_transformReduceInnermostDimIndex(THSYCLState *state,
                                        TensorTypeK *tgt1,
                                        TensorTypeIndex *tgt2,
                                        TensorTypeK *src,
                                        const std::pair<ScalarTypeK, ScalarTypeIndex>& init,
                                        BinaryFunction binary_op) {

  kernelTransformReduceInnermostDimIndex (
    tgt1->template data<ScalarTypeK>(),
    tgt2->template data<ScalarTypeIndex>(),
    src->template data<ScalarTypeK>(),
    tgt1, tgt2, src, init, binary_op);
}

template <typename ScalarTypeK,
          typename ScalarTypeIndex,
          typename TensorTypeK,
          typename TensorTypeIndex,
          typename BinaryFunction>
DP_HOST void
THSYCL_reduceDimIndex(THSYCLState *state,
                   TensorTypeK *tgt1_,
                   TensorTypeIndex *tgt2_,
                   TensorTypeK *src,
                   int64_t dimension,
                   int keepdim,
                   const std::pair<ScalarTypeK, ScalarTypeIndex> init,
                   BinaryFunction binary_op)
{
  THArgCheck(dimension >= 0 &&
             dimension < THSYCLTensor_nDimensionLegacyAll(state, src),
             3, "dimension out of range");

  // Unsqueeze tgt1_/tgt_2 if necessary so that their contiguity traits
  // are preserved if they are the same size as the correct reduction output.
  int src_dims = THSYCLTensor_nDimensionLegacyAll(state, src);
  THSYCLTensor_preserveReduceDimSemantics(
      state, tgt1_, src_dims, dimension, keepdim);
  THSYCLTensor_preserveReduceDimSemantics(
      state, tgt2_, src_dims, dimension, keepdim);

  std::vector<int64_t> dim = THTensor_sizesLegacyNoScalars(src);
  dim[dimension] = 1;
  THSYCLTensor_resize(state, tgt1_, dim, {});
  THSYCLTensor_resize(state, tgt2_, dim, {});

  TensorTypeK *tgt1 = (TensorTypeK*)THSYCLTensor_newContiguous<ScalarTypeK>(state, tgt1_);
  TensorTypeIndex *tgt2 = (TensorTypeIndex*)THSYCLTensor_newContiguous<ScalarTypeIndex>(state, tgt2_);
  src = (TensorTypeK*)THSYCLTensor_newContiguous<ScalarTypeK>(state, src);

  if (dimension == THSYCLTensor_nDimensionLegacyAll(state, src) - 1) {
    THSYCL_transformReduceInnermostDimIndex(state, tgt1, tgt2, src, init, binary_op);
  } else {
    THSYCL_transformReduceOuterDimIndex(state, tgt1, tgt2, src, dimension, init, binary_op);
  }

  THSYCLTensor_free(state, src);
  THSYCLTensor_freeCopyTo<ScalarTypeK>(state, tgt1, tgt1_);
  THSYCLTensor_freeCopyTo<ScalarTypeIndex>(state, tgt2, tgt2_);
  if (!keepdim) {
    THSYCLTensor_squeeze1d(state, tgt1_, tgt1_, dimension);
    THSYCLTensor_squeeze1d(state, tgt2_, tgt2_, dimension);
  }
}

template <typename T, typename Index>
struct MaxValuePair {
  std::pair<T, Index> operator()(const std::pair<T, Index> a,
                                 const std::pair<T, Index> b) const{
    return (THSYCLNumerics<T>::ge(a.first, b.first) ||
            THSYCLNumerics<T>::isnan(a.first)) ? a : b;
  }
};

template <typename T, typename Index>
struct MinValuePair {
  std::pair<T, Index> operator()(const std::pair<T, Index> a,
                                 const std::pair<T, Index> b) const {
    return (THSYCLNumerics<T>::le(a.first, b.first) ||
            THSYCLNumerics<T>::isnan(a.first)) ? a : b;
  }
};

#endif
