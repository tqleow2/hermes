// This file is part of Hermes3D
//
// Copyright (c) 2009 hp-FEM group at the University of Nevada, Reno (UNR).
// Email: hpfem-group@unr.edu, home page: http://hpfem.org/.
//
// Hermes3D is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published
// by the Free Software Foundation; either version 2 of the License,
// or (at your option) any later version.
//
// Hermes3D is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Hermes3D; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#include "amesos.h"
#include "../callstack.h"

#ifdef HAVE_AMESOS
  #include <Amesos_ConfigDefs.h>
#endif

#ifdef HAVE_AMESOS
template<typename Scalar> Amesos AmesosSolver<Scalar>::factory;
#endif

// Amesos solver ///////////////////////////////////////////////////////////////////////////////////
template<typename Scalar>
AmesosSolver<Scalar>::AmesosSolver(const char *solver_type, EpetraMatrix<Scalar> *m, EpetraVector<Scalar> *rhs)
  : LinearSolver<Scalar>(HERMES_FACTORIZE_FROM_SCRATCH), m(m), rhs(rhs)
{
  _F_
#ifdef HAVE_AMESOS
  solver = factory.Create(solver_type, problem);
  assert(solver != NULL);
  // WARNING: Amesos does not use RCP to allocate the Amesos_BaseSolver, 
  //          so don't forget to delete it!
  //          ( Amesos.cpp, line 88, called from factory.Create(): 
  //            return new Amesos_Klu(LinearProblem); )
#else
  error(AMESOS_NOT_COMPILED);
#endif
}

template<typename Scalar>
AmesosSolver<Scalar>::~AmesosSolver()
{
  _F_
#ifdef HAVE_AMESOS
  delete solver;
#endif
}

template<typename Scalar>
bool AmesosSolver<Scalar>::is_available(const char *name)
{
  _F_
#ifdef HAVE_AMESOS
  return factory.Query(name);
#else
  return false;
#endif
}

template<typename Scalar>
void AmesosSolver<Scalar>::set_use_transpose(bool use_transpose)
{
  _F_
#ifdef HAVE_AMESOS
  solver->SetUseTranspose(use_transpose);
#endif
}

template<typename Scalar>
bool AmesosSolver<Scalar>::use_transpose()
{
  _F_
#ifdef HAVE_AMESOS
  return solver->UseTranspose();
#else
  return false;
#endif
}

template<>
bool AmesosSolver<double>::solve()
{
  _F_
#ifdef HAVE_AMESOS
  assert(m != NULL);
  assert(rhs != NULL);
  
  assert(m->size == rhs->size);
  
  TimePeriod tmr;  

  problem.SetOperator(m->mat);
  problem.SetRHS(rhs->vec);
  Epetra_Vector x(*rhs->std_map);
  problem.SetLHS(&x);

  if (!setup_factorization())
  {
    warning("AmesosSolver: LU factorization could not be completed");
    return false;
  }

  int status = solver->Solve();
  if (status != 0) 
  {
    error("AmesosSolver: Solution failed.");
    return false;
  }
  
  tmr.tick();
  this->time = tmr.accumulated();

  delete [] this->sln;
  this->sln = new double[m->size]; MEM_CHECK(this->sln);
  // copy the solution into sln vector
  memset(this->sln, 0, m->size * sizeof(double));
  
  for (unsigned int i = 0; i < m->size; i++) this->sln[i] = x[i];

  return true;
#else
  return false;
#endif
}

template<>
bool AmesosSolver<std::complex<double> >::solve()
{
  _F_
#ifdef HAVE_AMESOS
  assert(m != NULL);
  assert(rhs != NULL);
  
  assert(m->size == rhs->size);
  
  TimePeriod tmr;  

  error("AmesosSolver<Scalar>::solve() not yet implemented for complex problems");

  if (!setup_factorization())
  {
    warning("AmesosSolver: LU factorization could not be completed");
    return false;
  }

  int status = solver->Solve();
  if (status != 0) 
  {
    error("AmesosSolver: Solution failed.");
    return false;
  }
  
  tmr.tick();
  this->time = tmr.accumulated();

  delete [] this->sln;
  this->sln = new std::complex<double>[m->size]; MEM_CHECK(this->sln);
  // copy the solution into sln vector
  memset(this->sln, 0, m->size * sizeof(std::complex<double>));
  
  return true;
#else
  return false;
#endif
}

template<typename Scalar>
bool AmesosSolver<Scalar>::setup_factorization()
{
  _F_
#ifdef HAVE_AMESOS
  // Perform both factorization phases for the first time.
  int eff_fact_scheme;
  if (this->factorization_scheme != HERMES_FACTORIZE_FROM_SCRATCH && 
      solver->NumSymbolicFact() == 0 && solver->NumNumericFact() == 0)
    eff_fact_scheme = HERMES_FACTORIZE_FROM_SCRATCH;
  else
    eff_fact_scheme = this->factorization_scheme;
  
  int status;
  switch(eff_fact_scheme)
  {
    case HERMES_FACTORIZE_FROM_SCRATCH:
      //debug_log("Factorizing symbolically.");
      status = solver->SymbolicFactorization();
      if (status != 0)
      {
        warning("Symbolic factorization failed.");
        return false;
      }
      
    case HERMES_REUSE_MATRIX_REORDERING:
    case HERMES_REUSE_MATRIX_REORDERING_AND_SCALING:
      status = solver->NumericFactorization();
      if (status != 0) 
      {
        warning("Numeric factorization failed.");
        return false;
      }
  }
  
  return true;
#else
  return false;
#endif
}

template class HERMES_API AmesosSolver<double>;
template class HERMES_API AmesosSolver<std::complex<double> >;
