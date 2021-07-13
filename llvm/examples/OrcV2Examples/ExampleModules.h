//===----- ExampleModules.h - IR modules for LLJIT examples -----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Example modules for LLJIT examples
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_EXAMPLES_ORCV2EXAMPLES_EXAMPLEMODULES_H
#define LLVM_EXAMPLES_ORCV2EXAMPLES_EXAMPLEMODULES_H

#include "llvm/ADT/StringRef.h"
#include "llvm/ExecutionEngine/Orc/ThreadSafeModule.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Error.h"
#include "llvm/Support/SourceMgr.h"

const llvm::StringRef Add1Example =
    R"(

; Function Attrs: noinline nounwind optnone ssp uwtable
define i32 @op1(i32 %0, i32 %1) {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 %0, i32* %3, align 4
  store i32 %1, i32* %4, align 4
  %5 = load i32, i32* %3, align 4
  %6 = load i32, i32* %4, align 4
  %7 = add nsw i32 %5, %6
  ret i32 %7
}

; Function Attrs: noinline nounwind optnone ssp uwtable
define i32 @op2(i32 %0, i32 %1)  {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 %0, i32* %3, align 4
  store i32 %1, i32* %4, align 4
  %5 = load i32, i32* %3, align 4
  %6 = load i32, i32* %4, align 4
  %7 = mul nsw i32 %5, %6
  ret i32 %7
}

; Function Attrs: noinline nounwind optnone ssp uwtable
define i32 @add1(i32 %0, i32 %1) {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store i32 %0, i32* %4, align 4
  store i32 %1, i32* %5, align 4
  %6 = load i32, i32* %4, align 4
  %7 = load i32, i32* %5, align 4
  %8 = sub nsw i32 %6, %7
  %9 = icmp sgt i32 %8, 0
  br i1 %9, label %10, label %14

10:                                               ; preds = %2
  %11 = load i32, i32* %4, align 4
  %12 = load i32, i32* %5, align 4
  %13 = call i32 @op1(i32 %11, i32 %12)
  store i32 %13, i32* %3, align 4
  br label %28

14:                                               ; preds = %2
  %15 = load i32, i32* %4, align 4
  %16 = load i32, i32* %5, align 4
  %17 = srem i32 %15, %16
  %18 = icmp eq i32 %17, 2
  br i1 %18, label %19, label %23

19:                                               ; preds = %14
  %20 = load i32, i32* %4, align 4
  %21 = load i32, i32* %5, align 4
  %22 = call i32 @op2(i32 %20, i32 %21)
  store i32 %22, i32* %3, align 4
  br label %28

23:                                               ; preds = %14
  br label %24

24:                                               ; preds = %23
  %25 = load i32, i32* %4, align 4
  %26 = load i32, i32* %5, align 4
  %27 = sub nsw i32 %25, %26
  store i32 %27, i32* %3, align 4
  br label %28

28:                                               ; preds = %24, %19, %10
  %29 = load i32, i32* %3, align 4
  ret i32 %29
}

)";

inline llvm::Error createSMDiagnosticError(llvm::SMDiagnostic &Diag) {
  using namespace llvm;
  std::string Msg;
  {
    raw_string_ostream OS(Msg);
    Diag.print("", OS);
  }
  return make_error<StringError>(std::move(Msg), inconvertibleErrorCode());
}

inline llvm::Expected<llvm::orc::ThreadSafeModule>
parseExampleModule(llvm::StringRef Source, llvm::StringRef Name) {
  using namespace llvm;
  auto Ctx = std::make_unique<LLVMContext>();
  SMDiagnostic Err;
  if (auto M = parseIR(MemoryBufferRef(Source, Name), Err, *Ctx))
    return orc::ThreadSafeModule(std::move(M), std::move(Ctx));

  return createSMDiagnosticError(Err);
}

inline llvm::Expected<llvm::orc::ThreadSafeModule>
parseExampleModuleFromFile(llvm::StringRef FileName) {
  using namespace llvm;
  auto Ctx = std::make_unique<LLVMContext>();
  SMDiagnostic Err;

  if (auto M = parseIRFile(FileName, Err, *Ctx))
    return orc::ThreadSafeModule(std::move(M), std::move(Ctx));

  return createSMDiagnosticError(Err);
}

#endif // LLVM_EXAMPLES_ORCV2EXAMPLES_EXAMPLEMODULES_H
