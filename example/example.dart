import 'dart:io';
import 'dart:ffi' as ffi;

import 'package:path/path.dart' as path;
import 'package:process_utils/process_utils.dart';

void main() {
  var libraryPath = path.join(
      Directory.current.path, 'process_utils', 'lib_process_utils.so'); // err
  if (Platform.isMacOS) {
    libraryPath = path.join(Directory.current.path, 'process_utils',
        'lib_process_utils.dylib'); // err
  } else if (Platform.isWindows) {
    libraryPath = path.join(Directory.current.path, 'process_utils', 'build',
        'lib_process_utils.dll'); // win
  }

  final dylib = ffi.DynamicLibrary.open(libraryPath);

  final utils = ProcessUtils(dylib);

  final String someImageName = "hello_world.exe";

  assert(utils.get_pid_by_name(someImageName) == -1);
  assert(utils.start_by_name(someImageName, '') == 1);
  assert(utils.get_pid_by_name(someImageName) != -1);
  assert(utils.kill_by_name(someImageName) == 1);
  assert(utils.get_pid_by_name(someImageName) == -1);
}
