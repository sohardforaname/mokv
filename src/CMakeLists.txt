aux_source_directory(./db DB_SRCS)
aux_source_directory(./storage STORAGE_SRCS)
aux_source_directory(./util UTIL_SRCS)

set(SRCS
    ${DB_SRCS}
    ${STORAGE_SRCS}
    ${UTIL_SRCS}
)

add_library(mokv ${SRCS})