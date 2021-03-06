=================
Welcome to Hermes
=================

.. raw:: latex
 
   \floatplacement{figure}{H}

Thank you for your interest in Hermes!

Hermes is a C++ library for rapid development of adaptive *hp*-FEM and *hp*-DG solvers,
with emphasis on nonlinear, time-dependent, multi-physics problems.

This document is organized as follows: 

* Section 1 provides general information about Hermes and the computational methods it uses,
  and gives references to underlying scientific articles.
* Section 2 describes how to install Hermes on various hardware platforms, and how to 
  install matrix solver packages and various optional packages. 
* Section 3 explains how to use Git and Github, and how you can contribute to the project if interested.

This document is under continuous development. If you find bugs, typos, dead links 
and such, please report them to the 
`Hermes2D mailing list <http://groups.google.com/group/hermes2d/>`_.

Introduction
------------

.. toctree::
    :maxdepth: 1

    src/about-hermes
    src/documentation
    src/math-background
    src/web-access
    src/citing-hermes

Installation
------------

.. toctree::
    :maxdepth: 1

    src/installation/linux
    src/installation/mac
    src/installation/win-cygwin
    src/installation/win-msvc
    src/installation/matrix_solvers
    src/installation/cython_installation
    src/installation/exodusII_netcdf

Collaboration
-------------

.. toctree::
    :maxdepth: 1

    src/collaboration
    src/editing_sphinx

Tutorial and Examples
---------------------

Tutorial and examples are in separate repositories. Before 
using them, make sure to install the Hermes library in your 
system::

    sudo make install

To get the Tutorial clone::

    git clone git://github.com/hpfem/hermes-tutorial.git

To get Examples, clone::

    git clone git://github.com/hpfem/hermes-examples.git

Both repositores contain a folder doc/ with documentation. To build 
the documentation, type "make html" in that directory.
