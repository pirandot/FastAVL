# FastAVL

Implementierung einer nicht trivialen AVL-Baum-Klasse in C++.

Template, dadurch flexibler.
Der Schlüsselwert (Key) ist Teil des AVL-Knotens,
der zusätzlich mögliche Wert (Val) wird geerbt.

Statt der sonst üblichen Knoten, die lediglich eine Zahl enthalten,
sind beispielsweise auch so etwas wie Springerpfade speicherbar
(Menge der übersprungenen Felder, Start- und Zielfeld bilden den Schlüssel),
deren zugehörige Klassengröße im zusätzlichen Wert gespeichert werden kann.
