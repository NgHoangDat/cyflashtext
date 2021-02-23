from distutils.core import setup, Extension

setup(
    name = 'flashtext',
    version = '0.1',
    author      = "datnh21",
    description = """Flashtext module""",
    ext_modules = [
        Extension(
            '_flashtext',
            sources=['flashtext_wrap.cxx', 'flashtext.cpp'],
        )
    ],
    py_modules = ["flashtext"],
)