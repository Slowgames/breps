
bazel build -c opt compilation_database

cat .\bazel-bin\compile_commands.json | sed -e "s,__EXEC_ROOT__,$(bazel info workspace),g" > .\compile_commands.json
