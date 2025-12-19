# flow_pipe_stage_example

This repository contains **example stage plugins** for **flow-pipe**.

It demonstrates how to:
- implement a custom **stage** in C++
- build it as a **shared library**
- install it so the **flow-pipe runtime** can load it dynamically
- reference the stage from a **Flow YAML spec**

There is **no runtime or controller code** in this repository — only reusable **stage plugins**.

---

## What is a Stage?

In flow-pipe, a **stage** is a unit of work inside a flow.  
Stages can be:

- **sources** – produce data
- **transforms** – modify data
- **sinks** – consume data
- **side-effects** – logging, metrics, notifications, etc.

Stages are compiled into **shared libraries** and loaded by the runtime at startup.

---

## Repository Layout

```
flow_pipe_stage_example/
├── CMakeLists.txt
├── Dockerfile
├── include/
│   └── flow_pipe_stage_example/
│       └── noop_stage.h
├── src/
│   └── noop_stage.cc
└── README.md
```

---

## Example Stage

This repo provides a simple example stage:

| Stage type | Plugin library |
|----------|----------------|
| `noop_stage` | `libstage_noop_stage.so` |

The stage does no processing — it simply passes data through — making it useful for learning and testing.

---

## Build Requirements

- C++20 compatible compiler
- CMake ≥ 3.22
- Docker (for container builds)

---

## Building the Docker Image

Build the image that contains the flow-pipe runtime and installs this stage plugin:

```bash
docker build -t flow-pipe-runtime-with-stages .
```

The resulting image includes:
- the compiled stage plugin installed at:
  ```
  /opt/flow-pipe/plugins
  ```
- the flow-pipe runtime binary

Flow definitions are **not** baked into the image and are supplied at runtime.

---

## Running with Docker

Provide flow definitions by mounting a local directory:

```bash
docker run --rm \
  -v $(pwd)/flows:/flows \
  flow-pipe-runtime-with-stages \
  /flows/my_flow.yaml
```

The runtime will:
- load stage plugins from:
  ```
  /opt/flow-pipe/plugins
  ```
- read flow definitions from:
  ```
  /flows
  ```

This keeps images immutable while allowing flows to be updated independently.

---

## Development Workflow (Recommended)

1. Build stage plugins into a Docker image
2. Publish or load the image into your environment
3. Run `flow_runtime` using that image (locally or in Kubernetes)
4. Update flows without rebuilding the runtime

This keeps:
- **runtime images self-contained**
- **deployments reproducible**
- **plugin loading predictable**

---

## Kubernetes Note

When running in Kubernetes:

- Stage plugins should be baked into the runtime image **or**
- Mounted via a volume

Expected plugin path inside the container:

```
/opt/flow-pipe/plugins
```

---

## Creating Your Own Stage

1. Create a new `.h` / `.cc` pair
2. Implement the stage interface
3. Export the registration symbol
4. Build a shared library named:

```
libstage_<your_stage_type>.so
```

Multiple stages may live in the same repository or be split across repositories.

---

## Versioning & Compatibility

- Stage plugins must be built against the **same flow-pipe SDK version** as the runtime
- ABI compatibility is **not guaranteed** across major versions

---

## Intended Use

This repository is intended for:
- learning how flow-pipe stages work
- bootstrapping custom stage development
- CI-built plugin artifacts shared across flows

---

## License

Apache 2.0

