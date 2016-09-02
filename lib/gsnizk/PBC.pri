# GS-NIZK project by Remi Bazin
# Implementation from the PBC library

LIBS        += -lgmp

pbc_static {
    LIBS        += pbc/libpbc.a
} else {
    LIBS        += -lpbc
}
