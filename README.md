# Multifrontal sovler for Finite Element Method meshes

Supported OS: Ubuntu, Mac OS X.

## Workflow generator

Requires node.js and docopt package (`npm install docopt`)


## Workflow tasks

### Requirements

* C++11 compatibile compiler (e.g. clang)
* LAPACKE (not CLAPACK!)
* LAPACK
* BLAS
* Boost (headers only)

On Ubuntu: `make ubuntu` to install required packages to build binaries.
On Mac OS X LAPACK, BLAS and suitable compiler are provided with OS, I'm not sure if Boost also is.

### Compiling

Just run `make`
