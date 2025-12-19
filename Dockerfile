# =====================================================
# Build example stage plugin (SDK-based)
# =====================================================

FROM ghcr.io/hurdad/flow-pipe-dev:main-ubuntu24.04 AS build

# Build deps only (SDK already installed)
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    ninja-build \
    clang \
 && rm -rf /var/lib/apt/lists/*

WORKDIR /build
COPY . .

RUN cmake -S . -B build -G Ninja \
      -DCMAKE_BUILD_TYPE=Release \
      -DFLOW_PIPE_ROOT=/opt/flow-pipe \
 && cmake --build build
