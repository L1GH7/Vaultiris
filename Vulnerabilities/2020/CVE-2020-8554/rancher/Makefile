# Image URL to use all building/pushing image targets
# k8s.gcr.io/multitenancy/externalip-webhook:v1.0.0
IMG ?= rancher/externalip-webhook:v1.0.0

default: all

.dapper:
	@echo Downloading dapper
	@curl -sL https://releases.rancher.com/dapper/latest/dapper-`uname -s`-`uname -m` > .dapper.tmp
	@@chmod +x .dapper.tmp
	@./.dapper.tmp -v
	@mv .dapper.tmp .dapper

rancher-build: .dapper
	./.dapper build

rancher-ci: .dapper
	./.dapper ci

rancher-chart-ci: .dapper
	./.dapper ./chart/ci

# Get the currently used golang install path (in GOPATH/bin, unless GOBIN is set)
ifeq (,$(shell go env GOBIN))
GOBIN=$(shell go env GOPATH)/bin
else
GOBIN=$(shell go env GOBIN)
endif

all: webhook

build: webhook

# Run tests
test: generate fmt vet lint manifests
	go test ./... -coverprofile cover.out

# Build webhook binary
webhook: generate fmt vet lint
	go build -o bin/webhook main.go

# Run against the configured Kubernetes cluster in ~/.kube/config
run: generate fmt vet manifests
	go run ./main.go

# Deploy webhook in the configured Kubernetes cluster in ~/.kube/config
deploy:
	cd config/webhook && kustomize edit set image webhook=${IMG}
	kustomize build config/default | kubectl apply -f -

# Generate manifests e.g. CRD, RBAC etc.
manifests: controller-gen
	$(CONTROLLER_GEN) $(CRD_OPTIONS) webhook paths="./..."

# Run go fmt against code
fmt:
	@echo "==> Checking that code complies with go fmt requirements..."
	@go fmt ./...

vet:
	@echo "==> Checking that code complies with go vet requirements..."
	@go vet $$(go list ./... | grep -v vendor/) ; if [ $$? -gt 0 ]; then \
		echo ""; \
		echo "Vet found suspicious constructs. Please check the reported constructs"; \
		echo "and fix them if necessary before submitting the code for review."; \
		exit 1; \
	fi
lint:
	@echo "==> Checking that code complies with golint requirements..."
	@GO111MODULE=off go get -u golang.org/x/lint/golint
	@if [ -n "$$(golint $$(go list ./...) | grep -v 'should have comment.*or be unexported' | tee /dev/stderr)" ]; then \
		echo ""; \
		echo "golint found style issues. Please check the reported issues"; \
		echo "and fix them if necessary before submitting the code for review."; \
    	exit 1; \
	fi

# Generate code
generate: controller-gen
	$(CONTROLLER_GEN) object:headerFile="hack/boilerplate.go.txt" paths="./..."

# Build the docker image
docker-build: test
	docker build . -t ${IMG}

# Push the docker image
docker-push:
	docker push ${IMG}

# find or download controller-gen
# download controller-gen if necessary
controller-gen:
ifeq (, $(shell which controller-gen))
	@{ \
	set -e ;\
	CONTROLLER_GEN_TMP_DIR=$$(mktemp -d) ;\
	cd $$CONTROLLER_GEN_TMP_DIR ;\
	go mod init tmp ;\
	go get sigs.k8s.io/controller-tools/cmd/controller-gen@v0.2.5 ;\
	rm -rf $$CONTROLLER_GEN_TMP_DIR ;\
	}
CONTROLLER_GEN=$(GOBIN)/controller-gen
else
CONTROLLER_GEN=$(shell which controller-gen)
endif
