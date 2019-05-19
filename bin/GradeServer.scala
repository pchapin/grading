
import java.io._
import java.net._

object GradeServer {

  def main(args: Array[String]) {
    def output(w: Writer, text: String) {
      print(text)
      w.write(text)
    }

    val listeningSocket = new ServerSocket(8801)
    while (true) {
      val currentClient = listeningSocket.accept()
      val incoming = new BufferedReader(new InputStreamReader(currentClient.getInputStream()))
      val outgoing = new BufferedWriter(new OutputStreamWriter(currentClient.getOutputStream()))

      // Read client request (POST operation)
      var line: String = null
      var contentLength = 0
      while ( { line = incoming.readLine(); line != null && line != "" } ) {
        if (line.length >= 14 && line.substring(0, 14) == "Content-Length") {
          val fields = line.split(""":\s*""")
          contentLength = fields(1).toInt
        }
        println(line)
      }
      println()

      // Read the posted client content.
      var processStdIn = ""
      for (i <- 1 to contentLength) {
        val ch = incoming.read()
        processStdIn = processStdIn + ch.toChar.toString
      }
      print(processStdIn)

      println("\n")
      output(outgoing, "HTTP/1.1 200 OK\n")
      output(outgoing, "Content-Type: text/html\n")

      val processBuilder = new ProcessBuilder("Debug/gradebook")
      val env = processBuilder.environment
      env.put("CONTENT_LENGTH", contentLength.toString)
      val process = processBuilder.start()
      val processInput = new BufferedWriter(new OutputStreamWriter(process.getOutputStream()))
      val processOutput = new BufferedReader(new InputStreamReader(process.getInputStream()))
      processInput.write(processStdIn)
      processInput.flush()
      var processLine = ""
      var content = ""
      while ( { processLine = processOutput.readLine(); processLine != null } ) {
        content = content + "\n" + processLine
      }

      output(outgoing, "Content-Length: " + content.length + "\n")
      output(outgoing, "\n")
      output(outgoing, content)
      outgoing.flush()

      currentClient.close()
    }
  }
}
