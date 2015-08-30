//var dbl = gui.showNumericDialog()
//var str = gui.showStringDialog()
//var fl = gui.showFileDialog()
//gui.showInfoDialog("Hello")
//gui.showPlotDialog()

console.write("write", "green")
console.write("write")
console.log_info("info")
console.log_debug("Debug")
console.log_warning("Warning")
console.log_error("ERROR")
console.log_fatal("FATAL")

console.write("set FC_1="+set.FC_1)
console.write("act FC_1="+act.FC_1)

set.val("FC_0", 23)
var fc0=act.FC_0
set.val("FC_1", fc0)
set.val("FC_2", set.FC_2 + 10)

//util.sleep(10000)
0
