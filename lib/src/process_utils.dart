// ignore_for_file: non_constant_identifier_names

import 'dart:ffi' as ffi;
import 'package:ffi/ffi.dart';

class ProcessUtils {
  final ffi.Pointer<T> Function<T extends ffi.NativeType>(String symbolName)
      _lookup;

  ProcessUtils(ffi.DynamicLibrary dynamicLibrary)
      : _lookup = dynamicLibrary.lookup;

  ProcessUtils.fromLookup(
      ffi.Pointer<T> Function<T extends ffi.NativeType>(String symbolName)
          lookup)
      : _lookup = lookup;

  int get_pid_by_name(String name) {
    ffi.Pointer<Utf8> namePtr = name.toNativeUtf8();
    final result = _get_pid_by_name(namePtr);
    calloc.free(namePtr);
    return result;
  }

  late final _get_pid_by_namePtr =
      _lookup<ffi.NativeFunction<ffi.Int Function(ffi.Pointer<Utf8>)>>(
          'get_pid_by_name');
  late final _get_pid_by_name =
      _get_pid_by_namePtr.asFunction<int Function(ffi.Pointer<Utf8>)>();
}
