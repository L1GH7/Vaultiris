package main

import "sync"

type XCLoginResult struct {
	Session   string `json:"session"`
	User      string `json:"user"`
	UserId    int    `json:"user_id"`
	ContextId int    `json:"context_id"`
	Locale    string `json:"locale"`
}

type XCStore interface {
	GetSession() (XCLoginResult, error)
	SetSession(session XCLoginResult)
}

type inMemoryXCStore struct {
	mu      sync.Mutex
	session *XCLoginResult
}

func NewInMemoryXCStore() XCStore {
	return &inMemoryXCStore{
		mu:      sync.Mutex{},
		session: nil,
	}
}

func (i *inMemoryXCStore) GetSession() (XCLoginResult, error) {
	i.mu.Lock()
	defer i.mu.Unlock()

	if i.session == nil {
		return XCLoginResult{}, ErrorInvalidSession
	}

	return *i.session, nil
}

func (i *inMemoryXCStore) SetSession(session XCLoginResult) {
	i.mu.Lock()
	defer i.mu.Unlock()

	i.session = &session
}
