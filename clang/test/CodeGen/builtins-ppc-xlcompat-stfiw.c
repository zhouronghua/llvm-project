// RUN: %clang_cc1 -O2 -triple powerpc64-unknown-unknown \
// RUN:   -emit-llvm %s -o - -target-cpu pwr9 | FileCheck %s
// RUN: %clang_cc1 -O2 -triple powerpc64le-unknown-unknown \
// RUN:   -emit-llvm %s -o - -target-cpu pwr9 | FileCheck %s
// RUN: %clang_cc1 -O2 -triple powerpc-unknown-aix \
// RUN:   -emit-llvm %s -o - -target-cpu pwr9 | FileCheck %s
// RUN: %clang_cc1 -O2 -triple powerpc64-unknown-aix \
// RUN:   -emit-llvm %s -o - -target-cpu pwr9 | FileCheck %s
// RUN: not %clang_cc1 -O2 -triple powerpc64-unknown-aix \
// RUN:   -emit-llvm %s -o - -target-cpu pwr8 2>&1 | \
// RUN:   FileCheck %s --check-prefix=CHECK-NON-PWR9-ERR

extern const int *cia;
extern double da;

void test_stfiw() {
  // CHECK-LABEL: test_stfiw
  // CHECK: void @llvm.ppc.stfiw(i8* %0, double %1)

  //  CHECK-NON-PWR9-ERR: error: this builtin is only valid on POWER9 or later CPUs
  __builtin_ppc_stfiw(cia, da);
}

void test_xl_stfiw() {
  // CHECK-LABEL: test_xl_stfiw
  // CHECK: void @llvm.ppc.stfiw(i8* %0, double %1)

  //  CHECK-NON-PWR9-ERR: error: this builtin is only valid on POWER9 or later CPUs
  __stfiw(cia, da);
}
