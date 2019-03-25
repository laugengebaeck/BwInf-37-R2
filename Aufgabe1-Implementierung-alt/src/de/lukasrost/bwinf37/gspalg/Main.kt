import de.lukasrost.bwinf37.gspalg.BestPathCalculator
import picocli.CommandLine
import picocli.CommandLine.Command
import picocli.CommandLine.Option
import picocli.CommandLine.Parameters
import java.io.File

@Command(name = "Aufgabe1", mixinStandardHelpOptions = true)
class Main : Runnable {
    @Option(
        names = ["-e", "--extension"],
        description = ["Erweiterung ausführen"]
    )
    private val extension = BooleanArray(0)

    @Parameters(arity = "1", paramLabel = "INPUT", description = ["Eingabe"])
    private var inputFile : File? = null
    @Parameters(arity = "1", paramLabel = "OUTPUT", description = ["Ausgabe"])
    private var outputFile : File? = null
    @Parameters(arity = "1", paramLabel = "SVG", description = ["SVG-Ausgabe"])
    private var svgFile : File? = null

    override fun run() {
        val bpc = BestPathCalculator()
        bpc.readRunSave(inputFile!!,outputFile!!,svgFile!!,extension.isNotEmpty())
    }

    companion object {

        @JvmStatic
        fun main(args: Array<String>) {
            CommandLine.run(Main(), *args)
        }
    }
}