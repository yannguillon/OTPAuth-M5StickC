function getNSData (archive, index) {
  return archive[0].$objects[index]['NS.data']
}

function getObjects (archive, index) {
  return archive[0].$objects[index]['NS.objects']
}

function getUID (archive, index, objectName) {
  return archive[0].$objects[index][objectName].UID
}

export {
  getNSData,
  getObjects,
  getUID
}
