#ifndef MPI_H_STANDALONE_FALLBACK
#define MPI_H_STANDALONE_FALLBACK

/*
 * MPI (Message Passing Interface) Standalone Fallback Header
 * 
 * Provides standard MPI-3 type definitions, constants, and function prototypes
 * so that distributed memory parallelism examples in this directory compile cleanly
 * under standard C++20 even when OpenMPI / MPICH is not installed.
 *
 * When compiling with an actual MPI implementation (mpic++), this header will be
 * bypassed in favor of the system MPI headers.
 */

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>

#ifdef __cplusplus
extern "C" {
#endif

// MPI Types
typedef int MPI_Comm;
typedef int MPI_Datatype;
typedef int MPI_Request;
typedef int MPI_Op;
typedef ptrdiff_t MPI_Aint;

// MPI Status Struct
typedef struct {
    int MPI_SOURCE;
    int MPI_TAG;
    int MPI_ERROR;
    int _count;
} MPI_Status;

// Communicators
#define MPI_COMM_NULL  0
#define MPI_COMM_WORLD 1
#define MPI_COMM_SELF  2

// Datatypes
#define MPI_DATATYPE_NULL 0
#define MPI_CHAR          1
#define MPI_INT           2
#define MPI_LONG          3
#define MPI_FLOAT         4
#define MPI_DOUBLE        5
#define MPI_BYTE          6

// Operations
#define MPI_OP_NULL 0
#define MPI_MAX     1
#define MPI_MIN     2
#define MPI_SUM     3
#define MPI_PROD    4
#define MPI_LAND    5
#define MPI_BAND    6
#define MPI_LOR     7
#define MPI_BOR     8

// Constants
#define MPI_SUCCESS          0
#define MPI_ANY_SOURCE     (-1)
#define MPI_ANY_TAG        (-1)
#define MPI_STATUS_IGNORE  ((MPI_Status*)0)
#define MPI_STATUSES_IGNORE ((MPI_Status*)0)

// Environment & Initialization
inline int MPI_Init(int* /*argc*/, char*** /*argv*/) { return MPI_SUCCESS; }
inline int MPI_Finalize(void) { return MPI_SUCCESS; }
inline int MPI_Initialized(int* flag) { if (flag) *flag = 1; return MPI_SUCCESS; }
inline int MPI_Finalized(int* flag) { if (flag) *flag = 0; return MPI_SUCCESS; }

// Communicator queries
inline int MPI_Comm_rank(MPI_Comm /*comm*/, int* rank) { if (rank) *rank = 0; return MPI_SUCCESS; }
inline int MPI_Comm_size(MPI_Comm /*comm*/, int* size) { if (size) *size = 1; return MPI_SUCCESS; }
inline int MPI_Comm_split(MPI_Comm /*comm*/, int /*color*/, int /*key*/, MPI_Comm* newcomm) {
    if (newcomm) *newcomm = MPI_COMM_WORLD;
    return MPI_SUCCESS;
}

// Point-to-Point Communication (Blocking)
inline int MPI_Send(const void* /*buf*/, int /*count*/, MPI_Datatype /*datatype*/,
                    int /*dest*/, int /*tag*/, MPI_Comm /*comm*/) {
    return MPI_SUCCESS;
}

inline int MPI_Recv(void* /*buf*/, int /*count*/, MPI_Datatype /*datatype*/,
                    int /*source*/, int /*tag*/, MPI_Comm /*comm*/, MPI_Status* /*status*/) {
    return MPI_SUCCESS;
}

// Point-to-Point Communication (Non-Blocking)
inline int MPI_Isend(const void* /*buf*/, int /*count*/, MPI_Datatype /*datatype*/,
                     int /*dest*/, int /*tag*/, MPI_Comm /*comm*/, MPI_Request* request) {
    if (request) *request = 0;
    return MPI_SUCCESS;
}

inline int MPI_Irecv(void* /*buf*/, int /*count*/, MPI_Datatype /*datatype*/,
                     int /*source*/, int /*tag*/, MPI_Comm /*comm*/, MPI_Request* request) {
    if (request) *request = 0;
    return MPI_SUCCESS;
}

inline int MPI_Wait(MPI_Request* /*request*/, MPI_Status* status) {
    if (status) {
        status->MPI_SOURCE = 0;
        status->MPI_TAG = 0;
        status->MPI_ERROR = MPI_SUCCESS;
        status->_count = 0;
    }
    return MPI_SUCCESS;
}

inline int MPI_Probe(int /*source*/, int /*tag*/, MPI_Comm /*comm*/, MPI_Status* status) {
    if (status) {
        status->MPI_SOURCE = 0;
        status->MPI_TAG = 0;
        status->MPI_ERROR = MPI_SUCCESS;
        status->_count = 100;
    }
    return MPI_SUCCESS;
}

inline int MPI_Get_count(const MPI_Status* status, MPI_Datatype /*datatype*/, int* count) {
    if (count) *count = status ? status->_count : 0;
    return MPI_SUCCESS;
}

// Collective Communication
inline int MPI_Bcast(void* /*buffer*/, int /*count*/, MPI_Datatype /*datatype*/,
                     int /*root*/, MPI_Comm /*comm*/) {
    return MPI_SUCCESS;
}

inline int MPI_Reduce(const void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype,
                      MPI_Op /*op*/, int /*root*/, MPI_Comm /*comm*/) {
    if (sendbuf && recvbuf) {
        size_t elem_size = (datatype == MPI_DOUBLE) ? sizeof(double) : sizeof(int);
        std::memcpy(recvbuf, sendbuf, count * elem_size);
    }
    return MPI_SUCCESS;
}

inline int MPI_Scatter(const void* sendbuf, int sendcount, MPI_Datatype datatype,
                       void* recvbuf, int recvcount, MPI_Datatype /*recvtype*/,
                       int /*root*/, MPI_Comm /*comm*/) {
    if (sendbuf && recvbuf) {
        size_t elem_size = (datatype == MPI_DOUBLE) ? sizeof(double) : sizeof(int);
        int copy_count = (sendcount < recvcount) ? sendcount : recvcount;
        std::memcpy(recvbuf, sendbuf, copy_count * elem_size);
    }
    return MPI_SUCCESS;
}

inline int MPI_Gather(const void* sendbuf, int sendcount, MPI_Datatype datatype,
                      void* recvbuf, int recvcount, MPI_Datatype /*recvtype*/,
                      int /*root*/, MPI_Comm /*comm*/) {
    if (sendbuf && recvbuf) {
        size_t elem_size = (datatype == MPI_DOUBLE) ? sizeof(double) : sizeof(int);
        int copy_count = (sendcount < recvcount) ? sendcount : recvcount;
        std::memcpy(recvbuf, sendbuf, copy_count * elem_size);
    }
    return MPI_SUCCESS;
}

// Topologies
inline int MPI_Dims_create(int nnodes, int ndims, int* dims) {
    if (dims && ndims > 0) {
        for (int i = 0; i < ndims; ++i) dims[i] = 1;
        dims[0] = nnodes;
    }
    return MPI_SUCCESS;
}

inline int MPI_Cart_create(MPI_Comm /*comm_old*/, int /*ndims*/, const int* /*dims*/,
                           const int* /*periods*/, int /*reorder*/, MPI_Comm* comm_cart) {
    if (comm_cart) *comm_cart = MPI_COMM_WORLD;
    return MPI_SUCCESS;
}

inline int MPI_Cart_coords(MPI_Comm /*comm*/, int /*rank*/, int maxdims, int* coords) {
    if (coords) {
        for (int i = 0; i < maxdims; ++i) coords[i] = 0;
    }
    return MPI_SUCCESS;
}

// User-Defined / Derived Datatypes
inline int MPI_Type_create_struct(int /*count*/, const int* /*array_of_blocklengths*/,
                                  const MPI_Aint* /*array_of_displacements*/,
                                  const MPI_Datatype* /*array_of_types*/,
                                  MPI_Datatype* newtype) {
    if (newtype) *newtype = 100;
    return MPI_SUCCESS;
}

inline int MPI_Type_commit(MPI_Datatype* /*datatype*/) { return MPI_SUCCESS; }
inline int MPI_Type_free(MPI_Datatype* /*datatype*/) { return MPI_SUCCESS; }

#ifdef __cplusplus
}
#endif

#endif // MPI_H_STANDALONE_FALLBACK
