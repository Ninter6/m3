#!/bin/bash
curl -fsSlO "https://raw.githubusercontent.com/Ninter6/m3/refs/heads/main/m3.ccm" && \
cat << EOF > m3.hpp && \
#ifndef M3_INC
#define M3_INC
#include "m3.ccm"
#undef M3_EXPORT
#endif
EOF
echo "m3 has been set up successfully" || \
echo "failed to set up m3"
