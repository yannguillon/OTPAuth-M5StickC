import JsCrypto, { Word32Array } from 'jscrypto'

function AESCBCDecrypt (data, key) {
  const params = new JsCrypto.CipherParams({ cipherText: new Word32Array(data) })
  return JsCrypto.AES.decrypt(params, key).toUint8Array()
}

function RNCryptorDecrypt (data, password) {
  const hmac = data.slice(data.length - 32)
  const headers = _parseHeaders(data)

  const headerLength = headers.length
  const cipherTextLength = data.length - headerLength - hmac.length
  const cipherText = data.slice(headerLength, headerLength + cipherTextLength)
  if (!validPassword(headers, hmac, cipherText, password)) return null

  const passwordKey = JsCrypto.PBKDF2.getKey(password, new Word32Array(headers.encryptionSalt), { keySize: 256 / 32, iterations: 10000, Hasher: JsCrypto.SHA1 })
  const content = new JsCrypto.CipherParams({ cipherText: new Word32Array(cipherText) })
  const decryptedData = JsCrypto.AES.decrypt(content, passwordKey, { iv: new Word32Array(headers.iv) })
  return decryptedData.toUint8Array()
}

const validPassword = function (headers, hmac, cipherText, password) {
  const hmacKey = JsCrypto.PBKDF2.getKey(password, new Word32Array(headers.hmacSalt), { keySize: 256 / 32, iterations: 10000, Hasher: JsCrypto.SHA1 })
  const concatArray = new Uint8Array([...headers.versionChar, ...headers.optionsChar, ...headers.encryptionSalt, ...headers.hmacSalt, ...headers.iv, ...cipherText])
  const hmacSHA1 = new JsCrypto.HmacSHA256(new Word32Array(concatArray), hmacKey)
  return hmacSHA1.toString(JsCrypto.Hex) === hmac.toString('hex')
}

const _parseHeaders = function (bufferData) {
  let offset = 0

  const versionChar = bufferData.slice(offset, offset + 1)
  offset += versionChar.length

  const optionsChar = bufferData.slice(offset, offset + 1)
  offset += optionsChar.length

  const encryptionSalt = bufferData.slice(offset, offset + 8)
  offset += encryptionSalt.length

  const hmacSalt = bufferData.slice(offset, offset + 8)
  offset += hmacSalt.length

  const iv = bufferData.slice(offset, offset + 16)
  offset += iv.length

  return {
    versionChar,
    optionsChar,
    encryptionSalt,
    hmacSalt,
    iv,
    length: offset
  }
}

export {
  AESCBCDecrypt,
  RNCryptorDecrypt
}
