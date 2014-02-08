#QtnProperty
This is user and programmist friendly properties in Qt
#Overview
There are some limitations of standard Qt property system.
This project is an attempt to make better properties for programmist and user.
Key features:
 * Properties hierarchy (properties can be organized in hierarchy at any depth)
 * Special property widget to observe and edit properties in uniform way
 * Scecial signals before and after property has changed
 * Property description - short text which help user to understand meaning and purpose of the property
 * Property state - property can be disabled or hidden at any moment
 * Serialization via QDataStream
 * Set/Get property value to QVariant and QString
 * Scripting support
 * Property delegates to customize look and feel property in property widget
 * PEG (property/enum generator) - it's tool like Qt moc which generates properties hierarchy from JSON like files.

This is screenshot of the Demo application

Property project (QtnPropertyCore and QtnPropertyWidget libs)
