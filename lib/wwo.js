'use strict'

const svm = require('node-svm')
const { randomInt } = require('./utils')

class Wave {
  constructor (hMax, value, features) {
    this.height = hMax
    this.value = value
    this.features = features
    this.fitness = 0
    this.length = 0.5
  }

  get binaryValue () {
    const binary = Number(Math.floor(this.value)).toString(2)
    return Array(this.features).fill('0').join('').substr(binary.length) + binary
  }

  get dimension () {
    return this.binaryValue.split('').map(x => +x).filter(x => !!x).length
  }

  async calcFitness (dataset) {
    const cls = new svm.CSVC()
    const features = this.binaryValue.split('').map(x => +x)
    const trainDB = dataset.map(({ values, feature }, i) => ({
      feature, values: values.filter((_, j) => features[j] > 0)
    }))

    return new Promise((resolve, reject) => {
      cls.train(trainDB.map(x => [x.values, x.feature])).done(() => {
        let hit = 0; let miss = 0
        trainDB.forEach(({ values, feature }) => {
          if (cls.predictSync(values) !== feature) { miss++ } else { hit++ }
        })

        this.fitness = (((hit / (hit + miss)) * 100) - this.dimension) / 100
        resolve()
      })
    })
  }

  async propagate (bestWave, dataset) {
    const waveLine = clone(this)
    waveLine.value = waveLine.value + randomInt(-1, 1) * waveLine.length * waveLine.dimension
    await waveLine.calcFitness(dataset)

    if (waveLine.fitness > this.fitness) {
      console.log('This is a better fitness.')
      Object.keys(this).forEach(i => { this[i] = waveLine[i] })
    }
  }
}

class WWO {
  constructor ({
    alpha = 0.01,
    beta = 0.001,
    populationSize = 10,
    hMax = 6
  } = {}) {
    this.alpha = alpha
    this.beta = beta
    this.populationSize = populationSize
    this.hMax = hMax
  }

  get dimension () {
    return this.dataset[0]['values'].length
  }

  get instances () {
    return this.dataset.length
  }

  async selectOptimalFeatures (dataset) {
    this.dataset = dataset
    this._showInfo()
    this._startPopulation()

    // Step 1: calc waves initial fitness
    await this._calcPopulationFitness()

    // Step 2: propagate waves
    await Promise.all(this.population.map(async (wave) => wave.propagate(null, this.dataset)))

    console.log(this.population)
  }

  _startPopulation () {
    this.population = Array(this.populationSize)
      .fill(undefined).map(() => new Wave(this.hMax, randomInt(0, Math.pow(2, this.dimension) - 1), this.dimension))
  }

  async _calcPopulationFitness () {
    return Promise.all(this.population.map(async (wave) => wave.calcFitness(this.dataset)))
  }

  _showInfo () {
    console.log(`WWO started for ${this.instances} instances with ${this.dimension} features.`)
    console.log(`Population: ${this.population}\tAlpha: ${this.alpha}\tBeta: ${this.beta}`)
  }
}

function clone (wave) {
  const waveClone = new Wave()
  Object.keys(waveClone).forEach(i => { waveClone[i] = wave[i] })
  return waveClone
}

module.exports = WWO
