import picocli.CommandLine
import picocli.CommandLine.Command
import picocli.CommandLine.Option
import picocli.CommandLine.Parameters
import java.io.File

@Command(name = "example", mixinStandardHelpOptions = true, version = "Picocli example 3.0")
class Example : Runnable {
    @Option(
        names = ["-v", "--verbose"],
        description = ["Verbose mode. Helpful for troubleshooting. " + "Multiple -v options increase the verbosity."]
    )
    private val verbose = BooleanArray(0)

    @Parameters(arity = "1", paramLabel = "INPUT", description = ["Input to process."])
    private val inputFile : File = null

    override fun run() {
        if (verbose.size > 0) {
            println(inputFiles!!.size.toString() + " files to process...")
        }
        if (verbose.size > 1) {
            for (f in inputFiles!!) {
                println(f.absolutePath)
            }
        }
    }

    companion object {

        @JvmStatic
        fun main(args: Array<String>) {
            CommandLine.run(Example(), *args)
        }
    }
}