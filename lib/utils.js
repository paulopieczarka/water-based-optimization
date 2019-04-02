function randomInt (min = 0, max = Number.MAX_SAFE_INTEGER) {
  max += 1
  return Math.floor(Math.random() * (max - min)) + min
}

module.exports = {
  randomInt
}
