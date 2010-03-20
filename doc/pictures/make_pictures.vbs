
Public Sub ExportAll()
    set Visio = GetObject("", "Visio.Application") 
    if Visio is Nothing then 
        wscript.echo "You need to install Microsoft Visio to start this script ..."
        wscript.exit
    end if
    Visio.Visible = True
    
    
    Pfad = replace(wscript.scriptfullname, wscript.scriptname, "") 
    PfadNew = Pfad + "png\"
    PfadExport = Pfad + "png\"
    
    set fso = CreateObject("Scripting.FileSystemObject")
    set picFolder = fso.GetFolder(Pfad)
    for each Dateiname in picFolder.Files
    
        if right(Dateiname, 4) = ".vsd" then 
            wscript.echo pfad + "\" + Dateiname
            Set docObj = Visio.Documents.Open(Dateiname)
            
            ' Über jede Page iterieren
            PageAnzahl = docObj.Pages.Count
        
            Set vcolors = docObj.Colors
        
            'On Error Resume Next
        
            For p = 1 To PageAnzahl
                Set pagObj = docObj.Pages.Item(p)
            
                ' pagObj.Export PfadExport + Mid$(Dateiname, 1, Len(Dateiname) - 4) + Str$(p) + ".wmf"
                pagObj.Export PfadExport + pagObj.Name + ".png"
            Next
                
            docObj.Saved = True
            docObj.Close
        end if
    next
End Sub

ExportAll