
env = Environment(CC = '/usr/bin/clang', CCFLAGS = ['-g ','-O2']) # initialize the Environment

env.Append(LIBS = 'm')

sources_main = Split("""
	main.c lattice.c mc.c progressbar.c
	""")

object_list = env.Object(source = sources_main)
env.Program(target = 'ising_2d', source = object_list)
