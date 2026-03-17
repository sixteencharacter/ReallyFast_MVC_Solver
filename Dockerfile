FROM ubuntu:22.04 AS builder
RUN apt-get update && apt-get install -y build-essential cmake libgomp1 libomp-dev

WORKDIR /app
COPY . /app
RUN cmake . 
RUN make

FROM ubuntu:22.04 AS runtime
RUN apt-get update  && apt-get install -y libgomp1
WORKDIR /app
COPY --from=builder /app/runner /app/runner
COPY startup.sh /app
# ENTRYPOINT ["/bin/sh","-c","\"/app/runner < \"$0\" > \"$1\"\""]
ENTRYPOINT ["/app/startup.sh"]
