FROM --platform=linux/amd64 ghcr.io/userver-framework/ubuntu-24.04-userver:latest AS builder
WORKDIR /app

RUN apt-get update && apt-get install -y cmake build-essential

COPY CMakeLists.txt .
COPY CMakePresets.json .
COPY cmake/ ./cmake/
COPY configs/ ./configs/

COPY src/ ./src/
COPY unittests/ ./unittests/

RUN cmake -B build -DCMAKE_BUILD_TYPE=Release
RUN cmake --build build -j$(nproc)

FROM --platform=linux/amd64 ghcr.io/userver-framework/ubuntu-24.04-userver:latest AS runtime
WORKDIR /app

COPY --from=builder /usr/local /usr/local
COPY --from=builder /app/build/service_template ./service_template
COPY configs/ ./configs/
ENV DB_CONNECTION=postgresql://postgres:postgres@db:5432/postgres
ENV LD_LIBRARY_PATH=/usr/local/lib
EXPOSE 8080

CMD ["./service_template", "--config", "configs/static_config.yaml", "--config_vars", "configs/config_vars.docker.yaml"]
