# Cargo

- Introduces two metadata files with various bits of package information.
- Fetches and builds your package’s dependencies.
- Invokes rustc or another build tool with the correct parameters to build your package.
- Introduces conventions to make working with Rust packages easier.

## installation

```sh
curl https://sh.rustup.rs -sSf | sh

## hello world
cargo new hello_world --bin --lib --vcs none
cargo build
cargo run
```

## dependencies

```toml
[dependencies]
time = "0.1.12"
regex = "0.1.41"
```

## package layout

```sh
.
├── Cargo.lock
├── Cargo.toml
├── src/
│  ├── lib.rs
│  ├── main.rs
│  └── bin/
│       ├── named-executable.rs
│       ├── another-executable.rs
│       └── multi-file-executable/
│         ├── main.rs
│         └── some_module.rs
├── benches/
│  ├── large-input.rs
│  └── multi-file-bench/
│      ├── main.rs
│      └── bench_module.rs
├── examples/
│  ├── simple.rs
│  └── multi-file-example/
│      ├── main.rs
│      └── ex_module.rs
└── tests/
    ├── some-integration-tests.rs
    └── multi-file-test/
        ├── main.rs
        └── test_module.rs
```

## .toml vs .lock

- `Cargo.toml` is about describing your dependencies in a broad sense, and is written by you.
- `Cargo.lock` contains exact information about your dependencies. It is maintained by Cargo and should not be manually edited.

## tests

Tests in `src` files should be unit tests, tests in `tests` should be integration-style tests.

## Continuous Integration

### Travis CI

```yml
language: rust
rust:
  - stable
  - beta
  - nightly
matrix:
  allow_failures:
    - rust: nightly
```

### Gitlab CI

```yml
stages:
  - build

rust-latest:
  stage: build
  image: rust:latest
  script:
    - cargo build --verbose
    - cargo test --verbose

rust-nightly:
  stage: build
  image: rustlang/rust:nightly
  script:
    - cargo build --verbose
    - cargo test --verbose
  allow_failure: true
```

## Specifying Dependencies

```toml
[dependencies]
time = "0.1.12"

^1.2.3  :=  >=1.2.3, <2.0.0
^1.2    :=  >=1.2.0, <2.0.0
^1      :=  >=1.0.0, <2.0.0
^0.2.3  :=  >=0.2.3, <0.3.0
^0.2    :=  >=0.2.0, <0.3.0
^0.0.3  :=  >=0.0.3, <0.0.4
^0.0    :=  >=0.0.0, <0.1.0
^0      :=  >=0.0.0, <1.0.0

~1.2.3  := >=1.2.3, <1.3.0
~1.2    := >=1.2.0, <1.3.0
~1      := >=1.0.0, <2.0.0

*     := >=0.0.0
1.*   := >=1.0.0, <2.0.0
1.2.* := >=1.2.0, <1.3.0
```

### Specifying dependencies from other registries

```toml
[dependencies]
some-crate = { version: "1.0", registry = "my-registry" }
regex = { git = "https://github.com/rust-lang/regex", branch = "next" }
hello_utils = { path = "hello_utils", version = "0.1.0" }
```

### Platform specific dependencies

```toml
[target.'cfg(windows)'.dependencies]
winhttp = "0.4.0"

[target.'cfg(unix)'.dependencies]
openssl = "1.0.1"

[target.'cfg(target_arch = "x86")'.dependencies]
native = { path = "native/i686" }

[target.'cfg(target_arch = "x86_64")'.dependencies]
native = { path = "native/x86_64" }
```
