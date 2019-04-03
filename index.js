const WWO = require('./lib/wwo')
const fs = require('fs')

const data = fs.readFileSync('./datasets/plat-sizes.data')

// const labels = { 'D1': 0, 'D2': 1, 'D3': 2, 'D4': 3 }

const dataset = data.toString().split('\n').map(x => {
  const values = x.split(',')
  const feature = values.pop()
  return { values: values.map(i => +i), feature: +feature }
})

const wwo = new WWO()
wwo.selectOptimalFeatures(dataset)
