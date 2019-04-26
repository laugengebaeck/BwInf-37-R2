DOKU = Aufgabe1-Dokumentation/Aufgabe1-Dokumentation.pdf Aufgabe2-Dokumentation/Aufgabe2-Dokumentation.pdf

zip: $(DOKU)
	zip -r BwInf-37.2-LukasRost.zip Aufgabe1-Implementierung Aufgabe2-Implementierung
	zip -j BwInf-37.2-LukasRost.zip Aufgabe1-Dokumentation/Aufgabe1-Dokumentation.pdf Aufgabe2-Dokumentation/Aufgabe2-Dokumentation.pdf
.PHONY: clean
clean:
	rm BwInf-37.2-LukasRost.zip
