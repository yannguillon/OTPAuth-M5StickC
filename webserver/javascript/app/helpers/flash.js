function createNotice (content) {
  window.dispatchEvent(new CustomEvent('flash', { detail: { type: 'notice', content } }))
}

function createAlert (content) {
  window.dispatchEvent(new CustomEvent('flash', { detail: { type: 'alert', content } }))
}

export {
  createNotice,
  createAlert
}
