import numpy, scipy, pylab

x = numpy.arange(-10, 10, 0.01)
y = numpy.sin(numpy.pi*x/4) / (numpy.pi*x/4)
z = numpy.sin(numpy.pi*x*4) / (numpy.pi*x*4 )

pylab.plot(x,y*y, linewidth=2.0, color=[0.8, 0.8, 0.8])

pylab.plot(x,z*y*y, color=[0.95, 0.7, 0.1])
pylab.axis('off')
pylab.savefig("sinc.svg")
pylab.show()


