from distutils.core import setup,Extension

mod=Extension('ci',
              sources=['ci.c','../gcca_rna.c'],
              extra_compile_args=['-w'],
              #extra_link_args=['../libgcca_rna.a'],
              language='c')

setup(name='ci',
      version= '0.1',
      description='ci package',
      ext_modules=[mod])

