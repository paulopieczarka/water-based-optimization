function randomInt (min = 0, max = Number.MAX_SAFE_INTEGER) {
  max += 1
  return Math.floor(Math.random() * (max - min)) + min
}

function generateGaussian (mean, std){
  const _2PI = Math.PI * 2
  var u1 = Math.random()
  var u2 = Math.random()
  var z0 = Math.sqrt(-2.0 * Math.log(u1)) * Math.cos(_2PI * u2)
  var z1 = Math.sqrt(-2.0 * Math.log(u1)) * Math.sin(_2PI * u2)
  return z0 * std + mean
}

module.exports = {
  randomInt,
  generateGaussian
}
