import parser
import ast

h = '''1,
5,
6,
8,
9,
87,
'''


print parser.expr('['+h+']')

exit(0)
try:
	#print parser.expr(h.replace('\n',''))
  print eval(h)
except Exception, e:
	print e.msg
	print ';;;'
	print e.args
	print ';;;'
	print e.lineno
	print ';;;'
	print e.message
	print ';;;'
	print e.offset
	print ';;;'
	print e.text
	print ';;;'
