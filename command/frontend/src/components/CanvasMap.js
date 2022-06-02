import React, {Component} from 'react';

export default class CanvasMap {
	constructor(tileSize) {
		this.tileSize = tileSize
		this.blank = this.#image("blank.png")
		this.rover = this.#image("rover.png")
		this.path = this.#image("path.png")
		this.mark = this.#image("mark.png")
	}

	#image(fileName) {
		const img = new Image()
		img.src = `images/${fileName}`
		return img
	}

	map = [[]]

	draw(canvas, ctx, map) {
		this.#setCanvasSize(canvas)
		this.#clearCanvas(canvas, ctx)
		this.#drawMap(ctx)
		this.map = map
	}

	#drawMap(ctx) {
		for (let row = 0; row < this.map.length; row++) {
			for (let column = 0; column < this.map[row].length; column++) {
				const tile = this.map[row][column]
				let image = null
				switch (tile) {
					case 0:
						image = this.blank
						break
					case 1:
						image = this.path
						break
					case 2:
						image = this.rover
						break
					case 3:
						image = this.mark
						break
				}

				if (image != null)
					ctx.drawImage(image, column * this.tileSize, row * this.tileSize, this.tileSize, this.tileSize)
			}
		}
	}

	#clearCanvas(canvas, ctx) {
		ctx.fillStyle = "black"
		ctx.fillRect(0, 0, canvas.width, canvas.height)
	}

	#setCanvasSize(canvas) {
		canvas.height = this.map.length * this.tileSize
		canvas.width = this.map[0].length * this.tileSize
	}
}
