## `.dig` – A new data format made for use with C
### Examples
#### Dialogue & Characters
```js
// characters.dig
// characters form Boneworks
#size 2
#unserialisable

HAYES
	$photo: "res/hayes.png"
	$name: "Hayes M."
	$age: 34

ARTHUR
	$photo: "res/arthur.png"
	$name: "Arthur F."
	$age: 33
```
```js
// dialogue.dig
// dialogue from Boneworks
#size 4
#unserialisable

STREETS_HAYES_1
	$text: "Yo, Arthur! Over here!"
	$character: HAYES
	$next: STREETS_HAYES_2

STREETS_HAYES_2
	$text: "How are you still in the city?"
	$character: HAYES
	$next: STREETS_HAYES_3

STREETS_HAYES_3
	$text: "The board's calling an emergency meeting for all Monogon employees."
	$character: HAYES
	$next: STREETS_HAYES_4

STREETS_HAYES_4
	$text: "All of MythOS is on lockdown, it's crazy over here."
	$character: HAYES
```