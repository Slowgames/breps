
new_http_archive(
  name = "compiledb",
  urls = ['https://github.com/grailbio/bazel-compilation-database/archive/0.2.tar.gz'],
  strip_prefix = 'bazel-compilation-database-0.2',
  build_file_content = """
filegroup(
  name="aspects",
  srcs=["aspects.bzl"],
  visibility=["//visibility:public"],
)
  """
)
