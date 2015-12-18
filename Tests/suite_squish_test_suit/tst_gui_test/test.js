function waitForChild() {
    if (arguments.length < 2)
        return null
    var obj = arguments[0]
    objectMap.add(obj)
    obj = waitForObject("{type='"+arguments[1]+"' parent='"+objectMap.symbolicName(obj)+"'}")
    for (var i = 2; i < arguments.length; i++) {
        objectMap.add(obj)
        obj = waitForObject("{type='"+arguments[i]+"' parent='"+objectMap.symbolicName(obj)+"'}")
    }
    return obj
}

function main() {
    var w = waitForObject("{type='QtnPropertyView' visible='1' window=':PropertyWidget Demo_MainWindow'}")
    var ap = w.accessibilityProxy()
    
    // test BoolProperty
    var p = ap.findProperty("BoolProperty")
    test.verify(!isNull(p), "BoolProperty should exists")
    // check property value
    test.compare(object.convertTo(p.value,"bool"), false)
    // check property delegate name
    test.compare(ap.propertyDelegateName(p), "default")
    // check property description
    test.compare(p.description, "Property to hold boolean values.")
    // get center of property edit rect
    var pt = ap.propertyActionRect(p, 0).center()
    mouseClick(w, pt.x, pt.y, Qt.NoModifier, Qt.LeftButton)
    snooze(1)
    // wait for popup checkbox widget
    var e = waitForChild(w, "QCheckBox")
    test.compare(e.checked, false)
    // modify property
    pt = e.rect.center()
    mouseClick(e, pt.x, pt.y, Qt.NoModifier, Qt.LeftButton)
    type(e, "<Return>")
    test.compare(object.convertTo(p.value,"bool"), true)
    snooze(1)
    
    // enable subproperty set
    p = ap.findProperty("EnableSubPropertySet")
    test.verify(!isNull(p))
    // get center of property name rect
    pt = ap.propertyNameRect(p).center()
    // activate property
    mouseClick(w, pt.x, pt.y, Qt.NoModifier, Qt.LeftButton)
    snooze(1)
    var p2 = ap.activeProperty()
    test.compare(p, p2, "EnableSubPropertySet should become active")

    // verify SubPropertySet is disabled
    p2 = ap.findProperty("SubPropertySet")
    test.verify(!isNull(p2))
    test.compare(p2.isEditable, false)
    test.compare(p2.state, 4) // QtnPropertyStateImmutable
    // start edit
    type(w, "<Return>")
    snooze(1)
    // wait for popup checkbox widget
    e = waitForChild(w, "QCheckBox")
    type(e, " ")
    type(e, "<Return>")
    // now SubPropertySet should be enabled
    test.compare(p2.isEditable, true)
    test.compare(p2.state, 0)
    
    // navigation by keyboard
    snooze(1)
    type(w, "<Down>")
    test.compare(ap.activeProperty().name, "SubPropertySet")
    snooze(1)
    // collapse property set by keyboard
    type(w, "<Left>")
    p = ap.findProperty("SubPropertySet.ReadOnlyString")
    test.verify(!isNull(p))
    test.compare(p.isEditable, false)
    test.compare(p.state, 12) // QtnPropertyStateImmutable | QtnPropertyStateCollapsed
    snooze(1)

    // expand property set by mouse
    pt = ap.propertyActionRect(ap.findProperty("SubPropertySet"), 0).center()
    mouseClick(w, pt.x, pt.y, Qt.NoModifier, Qt.LeftButton)
    test.compare(p.state, 4) // QtnPropertyStateImmutable
    snooze(1)
    
    //test string list delegate
    p = ap.findProperty("SubPropertySet.StringFromList")
    pt = ap.propertyActionRect(p, 0).center()
    mouseClick(w, pt.x, pt.y, Qt.NoModifier, Qt.LeftButton)
    e = waitForChild(w, "QComboBox")
    // get popup listbox
    e = e.view()
    snooze(1)
    clickItem(e, "four", 0, 0, Qt.NoModifier, Qt.LeftButton)
    // check property value
    test.compare(object.convertTo(p.value,"QString"), "four")
    snooze(1)

    p = ap.findProperty("SubPropertySet2.FileNameProperty")
    ap.ensureVisibleProperty(p)
    p = ap.propertyActionRect(p, 0).center()
    mouseClick(w, p.x, p.y, Qt.NoModifier, Qt.LeftButton)
    snooze(1)

    e = waitForChild(w, "QLineEdit")
    type(e, "hello from squish")
    type(e, "<Return>")
    snooze(3)

    return
}