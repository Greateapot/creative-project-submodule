// ignore_for_file: non_constant_identifier_names, unused_element, camel_case_types

import 'dart:ffi' as ffi;
import 'package:ffi/ffi.dart';

typedef _T_get_pid_by_name_native = ffi.Int Function(ffi.Pointer<Utf8>);
typedef _T_get_pid_by_name = int Function(ffi.Pointer<Utf8>);

typedef _T_kill_by_name_native = ffi.Int Function(ffi.Pointer<Utf8>);
typedef _T_kill_by_name = int Function(ffi.Pointer<Utf8>);

typedef _T_start_by_name_native = ffi.Int Function(
    ffi.Pointer<Utf8>, ffi.Pointer<Utf8>);
typedef _T_start_by_name = int Function(ffi.Pointer<Utf8>, ffi.Pointer<Utf8>);

class ProcessUtils {
  final ffi.Pointer<T> Function<T extends ffi.NativeType>(String symbolName)
      _lookup;

  ProcessUtils(ffi.DynamicLibrary dynamicLibrary)
      : _lookup = dynamicLibrary.lookup;

  ProcessUtils.fromLookup(
      ffi.Pointer<T> Function<T extends ffi.NativeType>(String symbolName)
          lookup)
      : _lookup = lookup;

  // get_pid_by_name
  int get_pid_by_name(String name) {
    ffi.Pointer<Utf8> namePtr = name.toNativeUtf8();
    final result = _get_pid_by_name(namePtr);
    calloc.free(namePtr);
    return result;
  }

  late final _get_pid_by_namePtr =
      _lookup<ffi.NativeFunction<_T_get_pid_by_name_native>>('get_pid_by_name');
  late final _get_pid_by_name =
      _get_pid_by_namePtr.asFunction<_T_get_pid_by_name>();
  // get_pid_by_name

  // kill_by_name
  int kill_by_name(String name) {
    ffi.Pointer<Utf8> namePtr = name.toNativeUtf8();
    final result = _kill_by_name(namePtr);
    calloc.free(namePtr);
    return result;
  }

  late final _kill_by_namePtr =
      _lookup<ffi.NativeFunction<_T_kill_by_name_native>>('kill_by_name');
  late final _kill_by_name = _kill_by_namePtr.asFunction<_T_kill_by_name>();
  // kill_by_name

  // start_by_name
  int start_by_name(String name, String args) {
    ffi.Pointer<Utf8> namePtr = name.toNativeUtf8();
    ffi.Pointer<Utf8> argsPtr = args.toNativeUtf8();
    final result = _start_by_name(namePtr, argsPtr);
    calloc.free(namePtr);
    calloc.free(argsPtr);
    return result;
  }

  late final _start_by_namePtr =
      _lookup<ffi.NativeFunction<_T_start_by_name_native>>('start_by_name');
  late final _start_by_name = _start_by_namePtr.asFunction<_T_start_by_name>();
  // start_by_name
}
