package main

import (
	"log"
	"net/http"
)

const (
	SessionIdLength = 32
)

type redirectorHandler struct {
	store RedirectorStore
}

func NewRedirectorHandler(store RedirectorStore) http.Handler {
	return &redirectorHandler{
		store: store,
	}
}

func (r *redirectorHandler) ServeHTTP(writer http.ResponseWriter, request *http.Request) {
	log.Printf("%s %s %s", request.Method, request.RequestURI, request.UserAgent())

	if request.Method != http.MethodGet {
		http.NotFound(writer, request)
		return
	}

	id := request.URL.Query().Get("session")
	if id == "" || len(id) != SessionIdLength {
		http.Error(writer, "bad session id", http.StatusBadRequest)
		return
	}

	session, err := r.store.Get(id)

	if err != nil {
		http.Error(writer, "invalid session", http.StatusBadRequest)
		return
	}

	if err := session.markValidated(); err == nil {
		log.Printf("marked session %s as validated", id)
		writer.WriteHeader(http.StatusOK)
		return
	}

	if err := session.markRedirected(); err == nil {
		log.Printf("marked session %s as redirected to %s", id, session.Target())
		http.Redirect(writer, request, session.Target(), http.StatusTemporaryRedirect)
		return
	}

	http.Error(writer, "request after session redirect complete", http.StatusBadRequest)
}

func startRedirector(address string, store RedirectorStore) {
	log.Printf("start redirect at %s", address)
	http.Handle("/redirect", NewRedirectorHandler(store))
	err := http.ListenAndServe(address, nil)
	AssertOk(err)
}
