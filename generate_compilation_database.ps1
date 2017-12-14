
bazel build -c opt compilation_database

cat .\bazel-bin\compile_commands.json | sed -e "s,__EXEC_ROOT__,$(bazel info execution_root),g" > .\compile_commands.json
