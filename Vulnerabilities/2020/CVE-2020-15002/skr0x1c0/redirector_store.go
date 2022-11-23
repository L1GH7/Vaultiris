package main

import (
	"fmt"
	"sync"
	"time"
)

var (
	ErrorNotValidated      = fmt.Errorf("not validated")
	ErrorAlreadyValidated  = fmt.Errorf("already validated")
	ErrorNotRedirected     = fmt.Errorf("not redirected")
	ErrorAlreadyRedirected = fmt.Errorf("already redirected")

	ErrorSessionNotPresent     = fmt.Errorf("session not present")
	ErrorSessionAlreadyPresent = fmt.Errorf("session already present")
)

type RedirectSession struct {
	mu             sync.Mutex
	target         string
	createTime     time.Time
	validationTime *time.Time
	redirectTime   *time.Time
}

func NewRedirectSession(redirectUrl string) *RedirectSession {
	return &RedirectSession{
		target:     redirectUrl,
		createTime: time.Now(),
	}
}

func (r *RedirectSession) markValidated() error {
	r.mu.Lock()
	defer r.mu.Unlock()

	if r.validationTime != nil {
		return ErrorAlreadyValidated
	}

	cTime := time.Now()
	r.validationTime = &cTime

	return nil
}

func (r *RedirectSession) markRedirected() error {
	r.mu.Lock()
	defer r.mu.Unlock()

	if r.redirectTime != nil {
		return ErrorAlreadyRedirected
	}

	cTime := time.Now()
	r.redirectTime = &cTime

	return nil
}

func (r *RedirectSession) Target() string {
	r.mu.Lock()
	defer r.mu.Unlock()

	return r.target
}

func (r *RedirectSession) CreateTime() time.Time {
	r.mu.Lock()
	defer r.mu.Unlock()
	return r.createTime
}

func (r *RedirectSession) ValidateTime() (time.Time, error) {
	r.mu.Lock()
	defer r.mu.Unlock()

	if t := r.validationTime; t != nil {
		return *t, nil
	}

	return time.Time{}, ErrorNotValidated
}

func (r *RedirectSession) RedirectTime() (time.Time, error) {
	r.mu.Lock()
	defer r.mu.Unlock()

	if t := r.redirectTime; t != nil {
		return *t, nil
	}

	return time.Time{}, ErrorNotRedirected
}

type RedirectorStore interface {
	Get(key string) (*RedirectSession, error)
	Set(key string, session *RedirectSession) error
}

type inMemoryRedirectorStore struct {
	mu    sync.Mutex
	store map[string]*RedirectSession
}

func NewInMemoryRedirectorStore() RedirectorStore {
	return &inMemoryRedirectorStore{
		store: make(map[string]*RedirectSession),
	}
}

func (i *inMemoryRedirectorStore) Get(key string) (*RedirectSession, error) {
	i.mu.Lock()
	defer i.mu.Unlock()

	if session, ok := i.store[key]; ok {
		return session, nil
	}

	return nil, ErrorSessionNotPresent
}

func (i *inMemoryRedirectorStore) Set(key string, session *RedirectSession) error {
	i.mu.Lock()
	defer i.mu.Unlock()

	if _, ok := i.store[key]; ok {
		return ErrorSessionAlreadyPresent
	}

	i.store[key] = session
	return nil
}
