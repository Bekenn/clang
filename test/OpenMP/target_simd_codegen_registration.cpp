// Test host codegen.
// RUN: %clang_cc1 -verify -fopenmp -fopenmp-version=45 -x c++ -triple powerpc64le-unknown-unknown -fopenmp-targets=powerpc64le-ibm-linux-gnu -emit-llvm %s -o - | FileCheck %s
// RUN: %clang_cc1 -fopenmp -fopenmp-version=45 -x c++ -std=c++11 -triple powerpc64le-unknown-unknown -fopenmp-targets=powerpc64le-ibm-linux-gnu -emit-pch -o %t %s
// RUN: %clang_cc1 -fopenmp -fopenmp-version=45 -x c++ -triple powerpc64le-unknown-unknown -fopenmp-targets=powerpc64le-ibm-linux-gnu -std=c++11 -include-pch %t -verify %s -emit-llvm -o - | FileCheck %s
// RUN: %clang_cc1 -verify -fopenmp -fopenmp-version=45 -x c++ -triple i386-unknown-unknown -fopenmp-targets=i386-pc-linux-gnu -emit-llvm %s -o - | FileCheck %s
// RUN: %clang_cc1 -fopenmp -fopenmp-version=45 -x c++ -std=c++11 -triple i386-unknown-unknown -fopenmp-targets=i386-pc-linux-gnu -emit-pch -o %t %s
// RUN: %clang_cc1 -fopenmp -fopenmp-version=45 -x c++ -triple i386-unknown-unknown -fopenmp-targets=i386-pc-linux-gnu -std=c++11 -include-pch %t -verify %s -emit-llvm -o - | FileCheck %s

// RUN: %clang_cc1 -verify -fopenmp-simd -fopenmp-version=45 -x c++ -triple powerpc64le-unknown-unknown -fopenmp-targets=powerpc64le-ibm-linux-gnu -emit-llvm %s -o - | FileCheck --check-prefix SIMD-ONLY0 %s
// RUN: %clang_cc1 -fopenmp-simd -fopenmp-version=45 -x c++ -std=c++11 -triple powerpc64le-unknown-unknown -fopenmp-targets=powerpc64le-ibm-linux-gnu -emit-pch -o %t %s
// RUN: %clang_cc1 -fopenmp-simd -fopenmp-version=45 -x c++ -triple powerpc64le-unknown-unknown -fopenmp-targets=powerpc64le-ibm-linux-gnu -std=c++11 -include-pch %t -verify %s -emit-llvm -o - | FileCheck --check-prefix SIMD-ONLY0 %s
// RUN: %clang_cc1 -verify -fopenmp-simd -fopenmp-version=45 -x c++ -triple i386-unknown-unknown -fopenmp-targets=i386-pc-linux-gnu -emit-llvm %s -o - | FileCheck --check-prefix SIMD-ONLY0 %s
// RUN: %clang_cc1 -fopenmp-simd -fopenmp-version=45 -x c++ -std=c++11 -triple i386-unknown-unknown -fopenmp-targets=i386-pc-linux-gnu -emit-pch -o %t %s
// RUN: %clang_cc1 -fopenmp-simd -fopenmp-version=45 -x c++ -triple i386-unknown-unknown -fopenmp-targets=i386-pc-linux-gnu -std=c++11 -include-pch %t -verify %s -emit-llvm -o - | FileCheck --check-prefix SIMD-ONLY0 %s
// SIMD-ONLY0-NOT: {{__kmpc|__tgt}}

// Test target simd codegen - host bc file has to be created first.
// RUN: %clang_cc1 -verify -fopenmp -fopenmp-version=45 -x c++ -triple powerpc64le-unknown-unknown -fopenmp-targets=powerpc64le-ibm-linux-gnu -emit-llvm-bc %s -o %t-ppc-host.bc
// RUN: %clang_cc1 -verify -fopenmp -fopenmp-version=45 -x c++ -triple powerpc64le-unknown-unknown -fopenmp-targets=powerpc64le-ibm-linux-gnu -emit-llvm %s -fopenmp-is-device -fopenmp-host-ir-file-path %t-ppc-host.bc -o - | FileCheck %s -check-prefix=TCHECK
// RUN: %clang_cc1 -fopenmp -fopenmp-version=45 -x c++ -std=c++11 -triple powerpc64le-unknown-unknown -fopenmp-targets=powerpc64le-ibm-linux-gnu -emit-pch -fopenmp-is-device -fopenmp-host-ir-file-path %t-ppc-host.bc -o %t %s
// RUN: %clang_cc1 -fopenmp -fopenmp-version=45 -x c++ -triple powerpc64le-unknown-unknown -fopenmp-targets=powerpc64le-ibm-linux-gnu -std=c++11 -fopenmp-is-device -fopenmp-host-ir-file-path %t-ppc-host.bc -include-pch %t -verify %s -emit-llvm -o - | FileCheck %s -check-prefix=TCHECK
// RUN: %clang_cc1 -verify -fopenmp -fopenmp-version=45 -x c++ -triple i386-unknown-unknown -fopenmp-targets=i386-pc-linux-gnu -emit-llvm-bc %s -o %t-x86-host.bc
// RUN: %clang_cc1 -verify -fopenmp -fopenmp-version=45 -x c++ -triple i386-unknown-unknown -fopenmp-targets=i386-pc-linux-gnu -emit-llvm %s -fopenmp-is-device -fopenmp-host-ir-file-path %t-x86-host.bc -o - | FileCheck %s -check-prefix=TCHECK
// RUN: %clang_cc1 -fopenmp -fopenmp-version=45 -x c++ -std=c++11 -triple i386-unknown-unknown -fopenmp-targets=i386-pc-linux-gnu -emit-pch -fopenmp-is-device -fopenmp-host-ir-file-path %t-x86-host.bc -o %t %s
// RUN: %clang_cc1 -fopenmp -fopenmp-version=45 -x c++ -triple i386-unknown-unknown -fopenmp-targets=i386-pc-linux-gnu -std=c++11 -fopenmp-is-device -fopenmp-host-ir-file-path %t-x86-host.bc -include-pch %t -verify %s -emit-llvm -o - | FileCheck %s -check-prefix=TCHECK

// RUN: %clang_cc1 -verify -fopenmp-simd -fopenmp-version=45 -x c++ -triple powerpc64le-unknown-unknown -fopenmp-targets=powerpc64le-ibm-linux-gnu -emit-llvm-bc %s -o %t-ppc-host.bc
// RUN: %clang_cc1 -verify -fopenmp-simd -fopenmp-version=45 -x c++ -triple powerpc64le-unknown-unknown -fopenmp-targets=powerpc64le-ibm-linux-gnu -emit-llvm %s -fopenmp-is-device -fopenmp-host-ir-file-path %t-ppc-host.bc -o - | FileCheck --check-prefix SIMD-ONLY1 %s
// RUN: %clang_cc1 -fopenmp-simd -fopenmp-version=45 -x c++ -std=c++11 -triple powerpc64le-unknown-unknown -fopenmp-targets=powerpc64le-ibm-linux-gnu -emit-pch -fopenmp-is-device -fopenmp-host-ir-file-path %t-ppc-host.bc -o %t %s
// RUN: %clang_cc1 -fopenmp-simd -fopenmp-version=45 -x c++ -triple powerpc64le-unknown-unknown -fopenmp-targets=powerpc64le-ibm-linux-gnu -std=c++11 -fopenmp-is-device -fopenmp-host-ir-file-path %t-ppc-host.bc -include-pch %t -verify %s -emit-llvm -o - | FileCheck --check-prefix SIMD-ONLY1 %s
// RUN: %clang_cc1 -verify -fopenmp-simd -fopenmp-version=45 -x c++ -triple i386-unknown-unknown -fopenmp-targets=i386-pc-linux-gnu -emit-llvm-bc %s -o %t-x86-host.bc
// RUN: %clang_cc1 -verify -fopenmp-simd -fopenmp-version=45 -x c++ -triple i386-unknown-unknown -fopenmp-targets=i386-pc-linux-gnu -emit-llvm %s -fopenmp-is-device -fopenmp-host-ir-file-path %t-x86-host.bc -o - | FileCheck --check-prefix SIMD-ONLY1 %s
// RUN: %clang_cc1 -fopenmp-simd -fopenmp-version=45 -x c++ -std=c++11 -triple i386-unknown-unknown -fopenmp-targets=i386-pc-linux-gnu -emit-pch -fopenmp-is-device -fopenmp-host-ir-file-path %t-x86-host.bc -o %t %s
// RUN: %clang_cc1 -fopenmp-simd -fopenmp-version=45 -x c++ -triple i386-unknown-unknown -fopenmp-targets=i386-pc-linux-gnu -std=c++11 -fopenmp-is-device -fopenmp-host-ir-file-path %t-x86-host.bc -include-pch %t -verify %s -emit-llvm -o - | FileCheck --check-prefix SIMD-ONLY1 %s
// SIMD-ONLY1-NOT: {{__kmpc|__tgt}}

// Check that no target code is emmitted if no omptests flag was provided.
// RUN: %clang_cc1 -verify -fopenmp -fopenmp-version=45 -x c++ -triple powerpc64le-unknown-unknown -emit-llvm %s -o - | FileCheck %s -check-prefix=CHECK-NTARGET

// RUN: %clang_cc1 -verify -fopenmp-simd -fopenmp-version=45 -x c++ -triple powerpc64le-unknown-unknown -emit-llvm %s -o - | FileCheck --check-prefix SIMD-ONLY2 %s
// SIMD-ONLY2-NOT: {{__kmpc|__tgt}}

// expected-no-diagnostics
#ifndef HEADER
#define HEADER

// CHECK-DAG: [[SA:%.+]] = type { [4 x i32] }
// CHECK-DAG: [[SB:%.+]] = type { [8 x i32] }
// CHECK-DAG: [[SC:%.+]] = type { [16 x i32] }
// CHECK-DAG: [[SD:%.+]] = type { [32 x i32] }
// CHECK-DAG: [[SE:%.+]] = type { [64 x i32] }
// CHECK-DAG: [[ST1:%.+]] = type { [228 x i32] }
// CHECK-DAG: [[ST2:%.+]] = type { [1128 x i32] }
// CHECK-DAG: [[ENTTY:%.+]] = type { i8*, i8*, i[[SZ:32|64]], i32, i32 }
// CHECK-DAG: [[DEVTY:%.+]] = type { i8*, i8*, [[ENTTY]]*, [[ENTTY]]* }
// CHECK-DAG: [[DSCTY:%.+]] = type { i32, [[DEVTY]]*, [[ENTTY]]*, [[ENTTY]]* }

// TCHECK:    [[ENTTY:%.+]] = type { i8*, i8*, i[[SZ:32|64]], i32, i32 }

// CHECK-DAG: $[[REGFN:\.omp_offloading\..+]] = comdat

// CHECK-DAG: [[A1:@.+]] = internal global [[SA]]
// CHECK-DAG: [[A2:@.+]] = global [[SA]]
// CHECK-DAG: [[B1:@.+]] = global [[SB]]
// CHECK-DAG: [[B2:@.+]] = global [[SB]]
// CHECK-DAG: [[C1:@.+]] = internal global [[SC]]
// CHECK-DAG: [[D1:@.+]] = global [[SD]]
// CHECK-DAG: [[E1:@.+]] = global [[SE]]
// CHECK-DAG: [[T1:@.+]] = global [[ST1]]
// CHECK-DAG: [[T2:@.+]] = global [[ST2]]

// CHECK-NTARGET-DAG: [[SA:%.+]] = type { [4 x i32] }
// CHECK-NTARGET-DAG: [[SB:%.+]] = type { [8 x i32] }
// CHECK-NTARGET-DAG: [[SC:%.+]] = type { [16 x i32] }
// CHECK-NTARGET-DAG: [[SD:%.+]] = type { [32 x i32] }
// CHECK-NTARGET-DAG: [[SE:%.+]] = type { [64 x i32] }
// CHECK-NTARGET-DAG: [[ST1:%.+]] = type { [228 x i32] }
// CHECK-NTARGET-DAG: [[ST2:%.+]] = type { [1128 x i32] }
// CHECK-NTARGET-NOT: type { i8*, i8*, %
// CHECK-NTARGET-NOT: type { i32, %

// We have 7 target regions

// CHECK-DAG: {{@.+}} = private constant i8 0
// TCHECK-NOT: {{@.+}} = private constant i8 0
// CHECK-DAG: {{@.+}} = private unnamed_addr constant [1 x i[[SZ]]] [i[[SZ]] 4]
// CHECK-DAG: {{@.+}} = private unnamed_addr constant [1 x i64] [i64 288]
// CHECK-DAG: {{@.+}} = private constant i8 0
// CHECK-DAG: {{@.+}} = private unnamed_addr constant [1 x i[[SZ]]] [i[[SZ]] 4]
// CHECK-DAG: {{@.+}} = private unnamed_addr constant [1 x i64] [i64 288]
// CHECK-DAG: {{@.+}} = private constant i8 0
// CHECK-DAG: {{@.+}} = private unnamed_addr constant [1 x i[[SZ]]] [i[[SZ]] 4]
// CHECK-DAG: {{@.+}} = private unnamed_addr constant [1 x i64] [i64 288]
// CHECK-DAG: {{@.+}} = private constant i8 0
// CHECK-DAG: {{@.+}} = private unnamed_addr constant [1 x i[[SZ]]] [i[[SZ]] 4]
// CHECK-DAG: {{@.+}} = private unnamed_addr constant [1 x i64] [i64 288]
// CHECK-DAG: {{@.+}} = private constant i8 0
// CHECK-DAG: {{@.+}} = private unnamed_addr constant [1 x i[[SZ]]] [i[[SZ]] 4]
// CHECK-DAG: {{@.+}} = private unnamed_addr constant [1 x i64] [i64 288]
// CHECK-DAG: {{@.+}} = private constant i8 0
// CHECK-DAG: {{@.+}} = private unnamed_addr constant [1 x i[[SZ]]] [i[[SZ]] 4]
// CHECK-DAG: {{@.+}} = private unnamed_addr constant [1 x i64] [i64 288]
// CHECK-DAG: {{@.+}} = private constant i8 0
// CHECK-DAG: {{@.+}} = private unnamed_addr constant [1 x i[[SZ]]] [i[[SZ]] 4]
// CHECK-DAG: {{@.+}} = private unnamed_addr constant [1 x i64] [i64 288]
// CHECK-DAG: {{@.+}} = private constant i8 0
// CHECK-DAG: {{@.+}} = private unnamed_addr constant [1 x i[[SZ]]] [i[[SZ]] 4]
// CHECK-DAG: {{@.+}} = private unnamed_addr constant [1 x i64] [i64 288]
// CHECK-DAG: {{@.+}} = private constant i8 0
// CHECK-DAG: {{@.+}} = private unnamed_addr constant [1 x i[[SZ]]] [i[[SZ]] 4]
// CHECK-DAG: {{@.+}} = private unnamed_addr constant [1 x i64] [i64 288]
// CHECK-DAG: {{@.+}} = private constant i8 0
// CHECK-DAG: {{@.+}} = private unnamed_addr constant [1 x i[[SZ]]] [i[[SZ]] 4]
// CHECK-DAG: {{@.+}} = private unnamed_addr constant [1 x i64] [i64 288]
// CHECK-DAG: {{@.+}} = private constant i8 0
// CHECK-DAG: {{@.+}} = private unnamed_addr constant [1 x i[[SZ]]] [i[[SZ]] 4]
// CHECK-DAG: {{@.+}} = private unnamed_addr constant [1 x i64] [i64 288]
// CHECK-DAG: {{@.+}} = private constant i8 0
// CHECK-DAG: {{@.+}} = private unnamed_addr constant [1 x i[[SZ]]] [i[[SZ]] 4]
// CHECK-DAG: {{@.+}} = private unnamed_addr constant [1 x i64] [i64 288]

// CHECK-NTARGET-NOT: private constant i8 0
// CHECK-NTARGET-NOT: private unnamed_addr constant [1 x i

// CHECK-DAG: [[NAMEPTR1:@.+]] = internal unnamed_addr constant [{{.*}} x i8] c"[[NAME1:__omp_offloading_[0-9a-f]+_[0-9a-f]+__Z.+_l[0-9]+]]\00"
// CHECK-DAG: [[ENTRY1:@.+]] = constant [[ENTTY]] { i8* @{{.*}}, i8* getelementptr inbounds ([{{.*}} x i8], [{{.*}} x i8]* [[NAMEPTR1]], i32 0, i32 0), i[[SZ]] 0, i32 0, i32 0 }, section ".omp_offloading.entries", align 1
// CHECK-DAG: [[NAMEPTR2:@.+]] = internal unnamed_addr constant [{{.*}} x i8] c"[[NAME2:.+]]\00"
// CHECK-DAG: [[ENTRY2:@.+]] = constant [[ENTTY]] { i8* @{{.*}}, i8* getelementptr inbounds ([{{.*}} x i8], [{{.*}} x i8]* [[NAMEPTR2]], i32 0, i32 0), i[[SZ]] 0, i32 0, i32 0 }, section ".omp_offloading.entries", align 1
// CHECK-DAG: [[NAMEPTR3:@.+]] = internal unnamed_addr constant [{{.*}} x i8] c"[[NAME3:.+]]\00"
// CHECK-DAG: [[ENTRY3:@.+]] = constant [[ENTTY]] { i8* @{{.*}}, i8* getelementptr inbounds ([{{.*}} x i8], [{{.*}} x i8]* [[NAMEPTR3]], i32 0, i32 0), i[[SZ]] 0, i32 0, i32 0 }, section ".omp_offloading.entries", align 1
// CHECK-DAG: [[NAMEPTR4:@.+]] = internal unnamed_addr constant [{{.*}} x i8] c"[[NAME4:.+]]\00"
// CHECK-DAG: [[ENTRY4:@.+]] = constant [[ENTTY]] { i8* @{{.*}}, i8* getelementptr inbounds ([{{.*}} x i8], [{{.*}} x i8]* [[NAMEPTR4]], i32 0, i32 0), i[[SZ]] 0, i32 0, i32 0 }, section ".omp_offloading.entries", align 1
// CHECK-DAG: [[NAMEPTR5:@.+]] = internal unnamed_addr constant [{{.*}} x i8] c"[[NAME5:.+]]\00"
// CHECK-DAG: [[ENTRY5:@.+]] = constant [[ENTTY]] { i8* @{{.*}}, i8* getelementptr inbounds ([{{.*}} x i8], [{{.*}} x i8]* [[NAMEPTR5]], i32 0, i32 0), i[[SZ]] 0, i32 0, i32 0 }, section ".omp_offloading.entries", align 1
// CHECK-DAG: [[NAMEPTR6:@.+]] = internal unnamed_addr constant [{{.*}} x i8] c"[[NAME6:.+]]\00"
// CHECK-DAG: [[ENTRY6:@.+]] = constant [[ENTTY]] { i8* @{{.*}}, i8* getelementptr inbounds ([{{.*}} x i8], [{{.*}} x i8]* [[NAMEPTR6]], i32 0, i32 0), i[[SZ]] 0, i32 0, i32 0 }, section ".omp_offloading.entries", align 1
// CHECK-DAG: [[NAMEPTR7:@.+]] = internal unnamed_addr constant [{{.*}} x i8] c"[[NAME7:.+]]\00"
// CHECK-DAG: [[ENTRY7:@.+]] = constant [[ENTTY]] { i8* @{{.*}}, i8* getelementptr inbounds ([{{.*}} x i8], [{{.*}} x i8]* [[NAMEPTR7]], i32 0, i32 0), i[[SZ]] 0, i32 0, i32 0 }, section ".omp_offloading.entries", align 1
// CHECK-DAG: [[NAMEPTR8:@.+]] = internal unnamed_addr constant [{{.*}} x i8] c"[[NAME8:.+]]\00"
// CHECK-DAG: [[ENTRY8:@.+]] = constant [[ENTTY]] { i8* @{{.*}}, i8* getelementptr inbounds ([{{.*}} x i8], [{{.*}} x i8]* [[NAMEPTR8]], i32 0, i32 0), i[[SZ]] 0, i32 0, i32 0 }, section ".omp_offloading.entries", align 1
// CHECK-DAG: [[NAMEPTR9:@.+]] = internal unnamed_addr constant [{{.*}} x i8] c"[[NAME9:.+]]\00"
// CHECK-DAG: [[ENTRY9:@.+]] = constant [[ENTTY]] { i8* @{{.*}}, i8* getelementptr inbounds ([{{.*}} x i8], [{{.*}} x i8]* [[NAMEPTR9]], i32 0, i32 0), i[[SZ]] 0, i32 0, i32 0 }, section ".omp_offloading.entries", align 1
// CHECK-DAG: [[NAMEPTR10:@.+]] = internal unnamed_addr constant [{{.*}} x i8] c"[[NAME10:.+]]\00"
// CHECK-DAG: [[ENTRY10:@.+]] = constant [[ENTTY]] { i8* @{{.*}}, i8* getelementptr inbounds ([{{.*}} x i8], [{{.*}} x i8]* [[NAMEPTR10]], i32 0, i32 0), i[[SZ]] 0, i32 0, i32 0 }, section ".omp_offloading.entries", align 1
// CHECK-DAG: [[NAMEPTR11:@.+]] = internal unnamed_addr constant [{{.*}} x i8] c"[[NAME11:.+]]\00"
// CHECK-DAG: [[ENTRY11:@.+]] = constant [[ENTTY]] { i8* @{{.*}}, i8* getelementptr inbounds ([{{.*}} x i8], [{{.*}} x i8]* [[NAMEPTR11]], i32 0, i32 0), i[[SZ]] 0, i32 0, i32 0 }, section ".omp_offloading.entries", align 1
// CHECK-DAG: [[NAMEPTR12:@.+]] = internal unnamed_addr constant [{{.*}} x i8] c"[[NAME12:.+]]\00"
// CHECK-DAG: [[ENTRY12:@.+]] = constant [[ENTTY]] { i8* @{{.*}}, i8* getelementptr inbounds ([{{.*}} x i8], [{{.*}} x i8]* [[NAMEPTR12]], i32 0, i32 0), i[[SZ]] 0, i32 0, i32 0 }, section ".omp_offloading.entries", align 1

// TCHECK-DAG: [[NAMEPTR1:@.+]] = internal unnamed_addr constant [{{.*}} x i8] c"[[NAME1:__omp_offloading_[0-9a-f]+_[0-9a-f]+__Z.+_l[0-9]+]]\00"
// TCHECK-DAG: [[ENTRY1:@.+]] = constant [[ENTTY]] { i8* bitcast (void (i[[SZ]])* @{{.*}} to i8*), i8* getelementptr inbounds ([{{.*}} x i8], [{{.*}} x i8]* [[NAMEPTR1]], i32 0, i32 0), i[[SZ]] 0, i32 0, i32 0 }, section ".omp_offloading.entries", align 1
// TCHECK-DAG: [[NAMEPTR2:@.+]] = internal unnamed_addr constant [{{.*}} x i8] c"[[NAME2:.+]]\00"
// TCHECK-DAG: [[ENTRY2:@.+]] = constant [[ENTTY]] { i8* bitcast (void (i[[SZ]])* @{{.*}} to i8*), i8* getelementptr inbounds ([{{.*}} x i8], [{{.*}} x i8]* [[NAMEPTR2]], i32 0, i32 0), i[[SZ]] 0, i32 0, i32 0 }, section ".omp_offloading.entries", align 1
// TCHECK-DAG: [[NAMEPTR3:@.+]] = internal unnamed_addr constant [{{.*}} x i8] c"[[NAME3:.+]]\00"
// TCHECK-DAG: [[ENTRY3:@.+]] = constant [[ENTTY]] { i8* bitcast (void (i[[SZ]])* @{{.*}} to i8*), i8* getelementptr inbounds ([{{.*}} x i8], [{{.*}} x i8]* [[NAMEPTR3]], i32 0, i32 0), i[[SZ]] 0, i32 0, i32 0 }, section ".omp_offloading.entries", align 1
// TCHECK-DAG: [[NAMEPTR4:@.+]] = internal unnamed_addr constant [{{.*}} x i8] c"[[NAME4:.+]]\00"
// TCHECK-DAG: [[ENTRY4:@.+]] = constant [[ENTTY]] { i8* bitcast (void (i[[SZ]])* @{{.*}} to i8*), i8* getelementptr inbounds ([{{.*}} x i8], [{{.*}} x i8]* [[NAMEPTR4]], i32 0, i32 0), i[[SZ]] 0, i32 0, i32 0 }, section ".omp_offloading.entries", align 1
// TCHECK-DAG: [[NAMEPTR5:@.+]] = internal unnamed_addr constant [{{.*}} x i8] c"[[NAME5:.+]]\00"
// TCHECK-DAG: [[ENTRY5:@.+]] = constant [[ENTTY]] { i8* bitcast (void (i[[SZ]])* @{{.*}} to i8*), i8* getelementptr inbounds ([{{.*}} x i8], [{{.*}} x i8]* [[NAMEPTR5]], i32 0, i32 0), i[[SZ]] 0, i32 0, i32 0 }, section ".omp_offloading.entries", align 1
// TCHECK-DAG: [[NAMEPTR6:@.+]] = internal unnamed_addr constant [{{.*}} x i8] c"[[NAME6:.+]]\00"
// TCHECK-DAG: [[ENTRY6:@.+]] = constant [[ENTTY]] { i8* bitcast (void (i[[SZ]])* @{{.*}} to i8*), i8* getelementptr inbounds ([{{.*}} x i8], [{{.*}} x i8]* [[NAMEPTR6]], i32 0, i32 0), i[[SZ]] 0, i32 0, i32 0 }, section ".omp_offloading.entries", align 1
// TCHECK-DAG: [[NAMEPTR7:@.+]] = internal unnamed_addr constant [{{.*}} x i8] c"[[NAME7:.+]]\00"
// TCHECK-DAG: [[ENTRY7:@.+]] = constant [[ENTTY]] { i8* bitcast (void (i[[SZ]])* @{{.*}} to i8*), i8* getelementptr inbounds ([{{.*}} x i8], [{{.*}} x i8]* [[NAMEPTR7]], i32 0, i32 0), i[[SZ]] 0, i32 0, i32 0 }, section ".omp_offloading.entries", align 1
// TCHECK-DAG: [[NAMEPTR8:@.+]] = internal unnamed_addr constant [{{.*}} x i8] c"[[NAME8:.+]]\00"
// TCHECK-DAG: [[ENTRY8:@.+]] = constant [[ENTTY]] { i8* bitcast (void (i[[SZ]])* @{{.*}} to i8*), i8* getelementptr inbounds ([{{.*}} x i8], [{{.*}} x i8]* [[NAMEPTR8]], i32 0, i32 0), i[[SZ]] 0, i32 0, i32 0 }, section ".omp_offloading.entries", align 1
// TCHECK-DAG: [[NAMEPTR9:@.+]] = internal unnamed_addr constant [{{.*}} x i8] c"[[NAME9:.+]]\00"
// TCHECK-DAG: [[ENTRY9:@.+]] = constant [[ENTTY]] { i8* bitcast (void (i[[SZ]])* @{{.*}} to i8*), i8* getelementptr inbounds ([{{.*}} x i8], [{{.*}} x i8]* [[NAMEPTR9]], i32 0, i32 0), i[[SZ]] 0, i32 0, i32 0 }, section ".omp_offloading.entries", align 1
// TCHECK-DAG: [[NAMEPTR10:@.+]] = internal unnamed_addr constant [{{.*}} x i8] c"[[NAME10:.+]]\00"
// TCHECK-DAG: [[ENTRY10:@.+]] = constant [[ENTTY]] { i8* bitcast (void (i[[SZ]])* @{{.*}} to i8*), i8* getelementptr inbounds ([{{.*}} x i8], [{{.*}} x i8]* [[NAMEPTR10]], i32 0, i32 0), i[[SZ]] 0, i32 0, i32 0 }, section ".omp_offloading.entries", align 1
// TCHECK-DAG: [[NAMEPTR11:@.+]] = internal unnamed_addr constant [{{.*}} x i8] c"[[NAME11:.+]]\00"
// TCHECK-DAG: [[ENTRY11:@.+]] = constant [[ENTTY]] { i8* bitcast (void (i[[SZ]])* @{{.*}} to i8*), i8* getelementptr inbounds ([{{.*}} x i8], [{{.*}} x i8]* [[NAMEPTR11]], i32 0, i32 0), i[[SZ]] 0, i32 0, i32 0 }, section ".omp_offloading.entries", align 1
// TCHECK-DAG: [[NAMEPTR12:@.+]] = internal unnamed_addr constant [{{.*}} x i8] c"[[NAME12:.+]]\00"
// TCHECK-DAG: [[ENTRY12:@.+]] = constant [[ENTTY]] { i8* bitcast (void (i[[SZ]])* @{{.*}} to i8*), i8* getelementptr inbounds ([{{.*}} x i8], [{{.*}} x i8]* [[NAMEPTR12]], i32 0, i32 0), i[[SZ]] 0, i32 0, i32 0 }, section ".omp_offloading.entries", align 1

// CHECK: [[ENTBEGIN:@.+]] = external constant [[ENTTY]]
// CHECK: [[ENTEND:@.+]] = external constant [[ENTTY]]
// CHECK: [[DEVBEGIN:@.+]] = external constant i8
// CHECK: [[DEVEND:@.+]] = external constant i8
// CHECK: [[IMAGES:@.+]] = internal unnamed_addr constant [1 x [[DEVTY]]] [{{.+}} { i8* [[DEVBEGIN]], i8* [[DEVEND]], [[ENTTY]]* [[ENTBEGIN]], [[ENTTY]]* [[ENTEND]] }], comdat($[[REGFN]])
// CHECK: [[DESC:@.+]] = internal constant [[DSCTY]] { i32 1, [[DEVTY]]* getelementptr inbounds ([1 x [[DEVTY]]], [1 x [[DEVTY]]]* [[IMAGES]], i32 0, i32 0), [[ENTTY]]* [[ENTBEGIN]], [[ENTTY]]* [[ENTEND]] }, comdat($[[REGFN]])

// We have 4 initializers, one for the 500 priority, another one for 501, or more for the default priority, and the last one for the offloading registration function.
// CHECK: @llvm.global_ctors = appending global [4 x { i32, void ()*, i8* }] [
// CHECK-SAME: { i32, void ()*, i8* } { i32 500, void ()* [[P500:@[^,]+]], i8* null },
// CHECK-SAME: { i32, void ()*, i8* } { i32 501, void ()* [[P501:@[^,]+]], i8* null },
// CHECK-SAME: { i32, void ()*, i8* } { i32 65535, void ()* [[PMAX:@[^,]+]], i8* null },
// CHECK-SAME: { i32, void ()*, i8* } { i32 0, void ()* bitcast (void (i8*)* @[[REGFN]] to void ()*), i8* bitcast (void (i8*)* @[[REGFN]] to i8*) }]

// CHECK-NTARGET: @llvm.global_ctors = appending global [3   x { i32, void ()*, i8* }] [

extern int *R;

struct SA {
  int arr[4];
  void foo() {
    int a = *R;
    a += 1;
    *R = a;
  }
  SA() {
    int a = *R;
    a += 2;
    *R = a;
  }
  ~SA() {
    int a = *R;
    a += 3;
    *R = a;
  }
};

struct SB {
  int arr[8];
  void foo() {
    int a = *R;
    #pragma omp target simd
    for (int i = 0; i < 10; ++i)
      a += 4;
    *R = a;
  }
  SB() {
    int a = *R;
    a += 5;
    *R = a;
  }
  ~SB() {
    int a = *R;
    a += 6;
    *R = a;
  }
};

struct SC {
  int arr[16];
  void foo() {
    int a = *R;
    a += 7;
    *R = a;
  }
  SC() {
    int a = *R;
    #pragma omp target simd
    for (int i = 0; i < 10; ++i)
      a += 8;
    *R = a;
  }
  ~SC() {
    int a = *R;
    a += 9;
    *R = a;
  }
};

struct SD {
  int arr[32];
  void foo() {
    int a = *R;
    a += 10;
    *R = a;
  }
  SD() {
    int a = *R;
    a += 11;
    *R = a;
  }
  ~SD() {
    int a = *R;
    #pragma omp target simd
    for (int i = 0; i < 10; ++i)
      a += 12;
    *R = a;
  }
};

struct SE {
  int arr[64];
  void foo() {
    int a = *R;
    #pragma omp target simd if(target: 0)
    for (int i = 0; i < 10; ++i)
      a += 13;
    *R = a;
  }
  SE() {
    int a = *R;
    #pragma omp target simd
    for (int i = 0; i < 10; ++i)
      a += 14;
    *R = a;
  }
  ~SE() {
    int a = *R;
    #pragma omp target simd
    for (int i = 0; i < 10; ++i)
      a += 15;
    *R = a;
  }
};

template <int x>
struct ST {
  int arr[128 + x];
  void foo() {
    int a = *R;
    #pragma omp target simd
    for (int i = 0; i < 10; ++i)
      a += 16 + x;
    *R = a;
  }
  ST() {
    int a = *R;
    #pragma omp target simd
    for (int i = 0; i < 10; ++i)
      a += 17 + x;
    *R = a;
  }
  ~ST() {
    int a = *R;
    #pragma omp target simd
    for (int i = 0; i < 10; ++i)
      a += 18 + x;
    *R = a;
  }
};

// We have to make sure we us all the target regions:
//CHECK-DAG: define internal void @[[NAME1]](
//CHECK-DAG: call void @[[NAME1]](
//CHECK-DAG: define internal void @[[NAME2]](
//CHECK-DAG: call void @[[NAME2]](
//CHECK-DAG: define internal void @[[NAME3]](
//CHECK-DAG: call void @[[NAME3]](
//CHECK-DAG: define internal void @[[NAME4]](
//CHECK-DAG: call void @[[NAME4]](
//CHECK-DAG: define internal void @[[NAME5]](
//CHECK-DAG: call void @[[NAME5]](
//CHECK-DAG: define internal void @[[NAME6]](
//CHECK-DAG: call void @[[NAME6]](
//CHECK-DAG: define internal void @[[NAME7]](
//CHECK-DAG: call void @[[NAME7]](
//CHECK-DAG: define internal void @[[NAME8]](
//CHECK-DAG: call void @[[NAME8]](
//CHECK-DAG: define internal void @[[NAME9]](
//CHECK-DAG: call void @[[NAME9]](
//CHECK-DAG: define internal void @[[NAME10]](
//CHECK-DAG: call void @[[NAME10]](
//CHECK-DAG: define internal void @[[NAME11]](
//CHECK-DAG: call void @[[NAME11]](
//CHECK-DAG: define internal void @[[NAME12]](
//CHECK-DAG: call void @[[NAME12]](

//TCHECK-DAG: define void @[[NAME1]](
//TCHECK-DAG: define void @[[NAME2]](
//TCHECK-DAG: define void @[[NAME3]](
//TCHECK-DAG: define void @[[NAME4]](
//TCHECK-DAG: define void @[[NAME5]](
//TCHECK-DAG: define void @[[NAME6]](
//TCHECK-DAG: define void @[[NAME7]](
//TCHECK-DAG: define void @[[NAME8]](
//TCHECK-DAG: define void @[[NAME9]](
//TCHECK-DAG: define void @[[NAME10]](
//TCHECK-DAG: define void @[[NAME11]](
//TCHECK-DAG: define void @[[NAME12]](

// CHECK-NTARGET-NOT: __tgt_target
// CHECK-NTARGET-NOT: __tgt_register_lib
// CHECK-NTARGET-NOT: __tgt_unregister_lib

// TCHECK-NOT: __tgt_target
// TCHECK-NOT: __tgt_register_lib
// TCHECK-NOT: __tgt_unregister_lib

// We have 2 initializers with priority 500
//CHECK: define internal void [[P500]](
//CHECK:     call void @{{.+}}()
//CHECK:     call void @{{.+}}()
//CHECK-NOT: call void @{{.+}}()
//CHECK:     ret void

// We have 1 initializers with priority 501
//CHECK: define internal void [[P501]](
//CHECK:     call void @{{.+}}()
//CHECK-NOT: call void @{{.+}}()
//CHECK:     ret void

// We have 6 initializers with default priority
//CHECK: define internal void [[PMAX]](
//CHECK:     call void @{{.+}}()
//CHECK:     call void @{{.+}}()
//CHECK:     call void @{{.+}}()
//CHECK:     call void @{{.+}}()
//CHECK:     call void @{{.+}}()
//CHECK:     call void @{{.+}}()
//CHECK-NOT: call void @{{.+}}()
//CHECK:     ret void

// Check registration and unregistration

//CHECK:     define internal void @[[UNREGFN:.+]](i8*)
//CHECK-SAME: comdat($[[REGFN]]) {
//CHECK:     call i32 @__tgt_unregister_lib([[DSCTY]]* [[DESC]])
//CHECK:     ret void
//CHECK:     declare i32 @__tgt_unregister_lib([[DSCTY]]*)

//CHECK:     define linkonce hidden void @[[REGFN]](i8*)
//CHECK-SAME: comdat {
//CHECK:     call i32 @__tgt_register_lib([[DSCTY]]* [[DESC]])
//CHECK:     call i32 @__cxa_atexit(void (i8*)* @[[UNREGFN]], i8* bitcast ([[DSCTY]]* [[DESC]] to i8*),
//CHECK:     ret void
//CHECK:     declare i32 @__tgt_register_lib([[DSCTY]]*)

static __attribute__((init_priority(500))) SA a1;
SA a2;
SB __attribute__((init_priority(500))) b1;
SB __attribute__((init_priority(501))) b2;
static SC c1;
SD d1;
SE e1;
ST<100> t1;
ST<1000> t2;


int bar(int a){
  int r = a;

  a1.foo();
  a2.foo();
  b1.foo();
  b2.foo();
  c1.foo();
  d1.foo();
  e1.foo();
  t1.foo();
  t2.foo();

  #pragma omp target simd
  for (int i = 0; i < 10; ++i)
    ++r;

  return r + *R;
}

// Check metadata is properly generated:
// CHECK:     !omp_offload.info = !{!{{[0-9]+}}, !{{[0-9]+}}, !{{[0-9]+}}, !{{[0-9]+}}, !{{[0-9]+}}, !{{[0-9]+}}, !{{[0-9]+}}, !{{[0-9]+}}, !{{[0-9]+}}, !{{[0-9]+}}, !{{[0-9]+}}, !{{[0-9]+}}}
// CHECK-DAG: = !{i32 0, i32 [[DEVID:-?[0-9]+]], i32 [[FILEID:-?[0-9]+]], !"_ZN2SB3fooEv", i32 216, i32 {{[0-9]+}}}
// CHECK-DAG: = !{i32 0, i32 [[DEVID]], i32 [[FILEID]], !"_ZN2SDD1Ev", i32 268, i32 {{[0-9]+}}}
// CHECK-DAG: = !{i32 0, i32 [[DEVID]], i32 [[FILEID]], !"_ZN2SEC1Ev", i32 286, i32 {{[0-9]+}}}
// CHECK-DAG: = !{i32 0, i32 [[DEVID]], i32 [[FILEID]], !"_ZN2SED1Ev", i32 293, i32 {{[0-9]+}}}
// CHECK-DAG: = !{i32 0, i32 [[DEVID]], i32 [[FILEID]], !"_ZN2STILi1000EE3fooEv", i32 305, i32 {{[0-9]+}}}
// CHECK-DAG: = !{i32 0, i32 [[DEVID]], i32 [[FILEID]], !"_ZN2STILi100EEC1Ev", i32 312, i32 {{[0-9]+}}}
// CHECK-DAG: = !{i32 0, i32 [[DEVID]], i32 [[FILEID]], !"_Z3bari", i32 436, i32 {{[0-9]+}}}
// CHECK-DAG: = !{i32 0, i32 [[DEVID]], i32 [[FILEID]], !"_ZN2STILi100EED1Ev", i32 319, i32 {{[0-9]+}}}
// CHECK-DAG: = !{i32 0, i32 [[DEVID]], i32 [[FILEID]], !"_ZN2STILi1000EEC1Ev", i32 312, i32 {{[0-9]+}}}
// CHECK-DAG: = !{i32 0, i32 [[DEVID]], i32 [[FILEID]], !"_ZN2STILi1000EED1Ev", i32 319, i32 {{[0-9]+}}}
// CHECK-DAG: = !{i32 0, i32 [[DEVID]], i32 [[FILEID]], !"_ZN2STILi100EE3fooEv", i32 305, i32 {{[0-9]+}}}
// CHECK-DAG: = !{i32 0, i32 [[DEVID]], i32 [[FILEID]], !"_ZN2SCC1Ev", i32 242, i32 {{[0-9]+}}}

// TCHECK:     !omp_offload.info = !{!{{[0-9]+}}, !{{[0-9]+}}, !{{[0-9]+}}, !{{[0-9]+}}, !{{[0-9]+}}, !{{[0-9]+}}, !{{[0-9]+}}, !{{[0-9]+}}, !{{[0-9]+}}, !{{[0-9]+}}, !{{[0-9]+}}, !{{[0-9]+}}}
// TCHECK-DAG: = !{i32 0, i32 [[DEVID:-?[0-9]+]], i32 [[FILEID:-?[0-9]+]], !"_ZN2SB3fooEv", i32 216, i32 {{[0-9]+}}}
// TCHECK-DAG: = !{i32 0, i32 [[DEVID]], i32 [[FILEID]], !"_ZN2SDD1Ev", i32 268, i32 {{[0-9]+}}}
// TCHECK-DAG: = !{i32 0, i32 [[DEVID]], i32 [[FILEID]], !"_ZN2SEC1Ev", i32 286, i32 {{[0-9]+}}}
// TCHECK-DAG: = !{i32 0, i32 [[DEVID]], i32 [[FILEID]], !"_ZN2SED1Ev", i32 293, i32 {{[0-9]+}}}
// TCHECK-DAG: = !{i32 0, i32 [[DEVID]], i32 [[FILEID]], !"_ZN2STILi1000EE3fooEv", i32 305, i32 {{[0-9]+}}}
// TCHECK-DAG: = !{i32 0, i32 [[DEVID]], i32 [[FILEID]], !"_ZN2STILi100EEC1Ev", i32 312, i32 {{[0-9]+}}}
// TCHECK-DAG: = !{i32 0, i32 [[DEVID]], i32 [[FILEID]], !"_Z3bari", i32 436, i32 {{[0-9]+}}}
// TCHECK-DAG: = !{i32 0, i32 [[DEVID]], i32 [[FILEID]], !"_ZN2STILi100EED1Ev", i32 319, i32 {{[0-9]+}}}
// TCHECK-DAG: = !{i32 0, i32 [[DEVID]], i32 [[FILEID]], !"_ZN2STILi1000EEC1Ev", i32 312, i32 {{[0-9]+}}}
// TCHECK-DAG: = !{i32 0, i32 [[DEVID]], i32 [[FILEID]], !"_ZN2STILi1000EED1Ev", i32 319, i32 {{[0-9]+}}}
// TCHECK-DAG: = !{i32 0, i32 [[DEVID]], i32 [[FILEID]], !"_ZN2STILi100EE3fooEv", i32 305, i32 {{[0-9]+}}}
// TCHECK-DAG: = !{i32 0, i32 [[DEVID]], i32 [[FILEID]], !"_ZN2SCC1Ev", i32 242, i32 {{[0-9]+}}}

#endif
