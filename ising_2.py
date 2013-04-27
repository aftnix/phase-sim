import numpy as np

#   Class Ising_lattice contains methods to explore the 2D Ising Model   
#   
#   Python doesn't support the "information hiding" aspects of
#   encapsulation, i.e. a separation of the notion of private vs public
#   data and methods. Private attributes and methods are conceptually
#   parts of an instance of a class that a "user" should not directly
#   manipulate; rather, this is done through methods that "know" how
#   to correctly do this. For example, after initialization of the
#   the Ising_lattice, the viarable _N denoting the size of the
#   lattice should *not* be changed manually ... that would create an 
#   inconsistency between the _spins array and _N, which many
#   of the methods use. If we wanted to dynamically change N, that
#   should be done through an appropriate method.
#   
#   We will follow a convention to mimic encapsulation, namely 
#   all attributes/methods preceded by an underscore _ should not
#   be altered by the user. The language doesn't prevent you from
#   doing so, but if you do, it's at your own risk ... i.e., you
#   could place an instance of a class in an inconsistent state.

class Ising_lattice:
   """
   Class to explore a simple nearest-neighbour 2D Ising model.
   Specifically, this contains a square, NxN periodic lattice,
   where each site contains a particle with spin S (+-1), and the 
   energy of the system is 
    
   E = - J Sum_{i,j} [ S_{i,j}*(S_{i+1,j}+S_{i,j+1}) ]
       - H Sum_{i,j} [ S_{i,j} ]
    
   The periodic identification is site [i,j] = site [i+m*N,j+l*N], where m,l are integers
    
   This class has no public attributes.
    
   Public methods: 
   ===============
    
   __init__(self,N,J,H) : by default calls aligned_spins(1)
    
   lattice initialization:
   -----------------------
    
   random_spins(self) 
   aligned_spins(self,S) : sets all spins to S (+- 1)
    
   lattice manipulation: 
   ---------------------
    
   spin_flip(i,j) : flips the (i,j)^th spin and returns dE
   cond_spin_flip(i,j,T) : flips (i,j)^th spin subject to the Metropolis-Hastings, conditions: 
                              (1) always if dE < 0
                              (2) with probability exp^(-dE/T) if dE>0, (Boltzman's constant k=1 here)
                              Returns dE
   query: 
   ------
    
   N(): 
   E(): 
   M():
   H():
   J(): 
   spin_ij(i,j): single spin
   spin(): array of spins
   diagram(): prints spin array in characters  '@' (up) and ' ' (down)

   """

   #  Private attributes and methods
   #   _N : dimension of lattice
   #   _J : characterizes the interaction energy
   #   _H : an external magnetic field.
   #   _E : energy of lattice 
   #   _M : net magnetization
   #
   #   _spins : array of spins
   #
   #   _compute_E_M : recomputes _E and _M over entire lattice

   # Initialization methods

   def __init__(self,N,J=1.0,H=0.0):
      self._N=N
      self._J=J
      self._H=H
      self.aligned_spins()
      self._compute_E_M()

   def aligned_spins(self,S=1):
      if (not(S==1 or S==-1)):
         print "Ising_lattice.aligned_spins: Error, spin must be +=1"
         raise(AttributeError)
      self._spins=np.ones((self._N,self._N),dtype=int)*S
      self._compute_E_M()
    
   def random_spins(self):
      self._spins=np.where(np.random.random((self._N,self._N))>0.5,1,-1)
      self._compute_E_M()

   # Query methods
    
   def N(self):
      return self._N
    
   def M(self):
      return self._M
    
   def H(self):
      return self._H
    
   def J(self):
      return self._J
    
   def E(self):
      return self._E
    
   def spins(self):

      # the following is a "trick" to help maintain the privacy of _spins.
      # _spins is mutable, and the 'return' without the ()*1 will simply
      # return a pointer to _spins, and so could be changed external to the
      # class. By multiplying by 1, a new array is created, and that is returned.
      return (self._spins)*1
    
   def spin_ij(self,i,j):
      return self._spins[i%self._N,j%self._N]
    
   def __str__(self):
      return '\nLattice properties: %d^2 cells, E=%f, M=%d, <E>=%f, <M>=%f\n'%\
             (self._N,self._E,self._M,self._E/self._N**2,self._M/self._N**2)
    
   def diagram(self):
      print self
      print np.where(self._spins>0, '@',' ')

   # Manipulation methods
    
   def spin_flip(self,i,j): 
      N=self._N
      s=self._spins
      dM=-2.0*s[i%N,j%N]
      dE=2.0*s[i%N,j%N]*self._H
      # N==1 is a special case ... particle is it's own neighbour, so *all* 
      # spins flip, self-interaction E doesn't change
      if (N>1):
         dE+=2*s[i%N,j%N]*self._J*(s[i%N,(j+1)%N]+s[(i+1)%N,j%N]+s[i%N,(j-1)%N]+s[(i-1)%N,j%N])
      self._spins[i%N,j%N]*=-1
      self._E+=dE
      self._M+=dM
      return dE
    
   def cond_spin_flip(self,i,j,T): 
      dE=self.spin_flip(i,j)
      if (dE<0.0 or (T>0.0 and (np.random.random()<np.exp(-dE/T)))): return dE
      self.spin_flip(i,j)
      return 0
    
   # private methods
    
   def _compute_E_M(self):
      self._M=np.sum(self._spins)*1.0
      self._E=0.0
      for i in range(self._N):
         for j in range(self._N):
            self._E-=self._spins[i,j]*(self._spins[i,(j+1)%self._N]+self._spins[(i+1)%self._N,j])
      self._E*=self._J
      self._E-=self._M*self._H
   #
 
