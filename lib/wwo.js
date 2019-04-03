'use strict'

const svm = require('node-svm')
const { randomInt, generateGaussian } = require('./utils')

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
      cls.train(trainDB.map(x => [x.values, x.feature])).done((report) => {
        // let hit = 0; let miss = 0
        // trainDB.forEach(({ values, feature }) => {
        //   if (cls.predictSync(values) !== feature) { miss++ } else { hit++ }
        // })

        // this.fitness = (((hit / (hit + miss)) * 100) - this.dimension) / 100
        this.fitness = (report[1]['accuracy'] * 100 - this.dimension) / 100
        resolve()
      })
    })
  }

  toString () {
    return `Wave { h:${this.height}, l:${this.length}, f:${this.fitness}, d:${this.dimension} }`
  }
}

class WWO {
  constructor ({
    alpha = 1.01,
    beta = 0.001,
    populationSize = 5,
    hMax = 6
  } = {}) {
    this.alpha = alpha
    this.beta = beta
    this.populationSize = populationSize
    this.hMax = hMax
    this.bestWave = null
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

    for (let i=0; i < 50; i++) {
      // Step 2: propagate waves
      await Promise.all(this.population.map(async (wave, k) => (
        this._propagate(k)
      )))

      this._updateWaveLengths()

      // console.log(`> #${i}\tbest wave has ${this.bestWave.dimension} features with accuracy of ${this.bestWave.fitness}`)
      console.log(`>${i}\t${this.bestWave.toString()}`)
    }

    const cls0 = new svm.CSVC()
    cls0.train(dataset.map(x => [x.values, x.feature])).done(report => {
      console.log('Before feature selection accuracy:', report[1]['accuracy'])
    })


    const cls1 = new svm.CSVC()
    const features = this.bestWave.binaryValue.split('').map(x => +x)
    const trainDB = this.dataset.map(({ values, feature }, i) => ({
      feature, values: values.filter((_, j) => features[j] > 0)
    }))

    cls1.train(trainDB.map(x => [x.values, x.feature])).done(report => {
      console.log('After feature selection accuracy:', report[1]['accuracy'])
    })

    // console.log(this.population)

    // console.log('\n', this.bestWave)
  }

  _startPopulation () {
    this.population = Array(this.populationSize)
      .fill(undefined).map(() => new Wave(this.hMax, randomInt(0, Math.pow(2, this.dimension) - 1), this.dimension))
  }

  async _calcPopulationFitness () {
    await Promise.all(this.population.map(async (wave) => wave.calcFitness(this.dataset)))
    this.bestWave = this.population[0]
    return this.population
  }

  async _propagate (k) {
    let waveLine = clone(this.population[k])
    waveLine.value = waveLine.value + randomInt(-1, 1) * waveLine.length * waveLine.features
    await waveLine.calcFitness(this.dataset)

    if (waveLine.fitness > this.population[k].fitness) {
      if (waveLine.fitness > this.bestWave.fitness) {
        console.log('> New best wave: ', this.bestWave.fitness, 'to', waveLine.fitness)
        waveLine = this._break(waveLine)
        this.bestWave = clone(waveLine)
      }

      this.population[k] = clone(waveLine)
    } else {
      this.population[k].height -= 1
      if (this.population[k].height === 0) {
        this.population[k] = this._refract(waveLine, this.population[k])
      }
    }
  }

  _break (wave) {
    const breakedWave = clone(wave)
    const { value, features } = breakedWave
    breakedWave.value = value + generateGaussian(0, 1) * this.beta * features
    return breakedWave
  }

  _refract (waveLine, wave) {
    const refractedWave = clone(waveLine)
    refractedWave.value = generateGaussian(
      (this.bestWave.value + wave.value) / 2,
      Math.abs(this.bestWave.value - wave.value) / 2
    )
    refractedWave.height = this.hMax
    refractedWave.length = wave.length * (wave.fitness / waveLine.fitness)
    return refractedWave
  }

  _updateWaveLengths () {
    const sortedWaves = [...this.population].sort((w0, w1) => w1.fitness - w0.fitness)
    const best = sortedWaves[0]
    const worst = sortedWaves.slice(-1).pop()

    this.population.forEach((wave, k) => {
      const exp = -(wave.fitness - worst.fitness + Math.E) / (best.fitness - worst.fitness + Math.E)
      this.population[k].length = wave.length * Math.pow(this.alpha, exp)
    })
  }

  _showInfo () {
    console.log(`WWO started for ${this.instances} instances with ${this.dimension} features.`)
    console.log(`Population: ${this.populationSize}\tAlpha: ${this.alpha}\tBeta: ${this.beta}`)
  }
}

function clone (wave) {
  const waveClone = new Wave()
  Object.keys(waveClone).forEach(i => { waveClone[i] = wave[i] })
  return waveClone
}

module.exports = WWO
