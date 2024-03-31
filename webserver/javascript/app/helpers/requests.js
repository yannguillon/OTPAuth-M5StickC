async function fetchWithOK (path, method, body) {
  const response = await fetch(path, {
    headers: { 'Content-Type': 'application/json' },
    method,
    body: JSON.stringify(body)
  })

  if (response.status !== 200) {
    throw new Error()
  }
  return response
}
export {
  fetchWithOK
}
