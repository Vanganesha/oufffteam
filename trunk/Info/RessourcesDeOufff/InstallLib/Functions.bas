Attribute VB_Name = "Functions"
Public Sub Step0()

    Dim EnvPath As String
    Dim Path As String
    Dim EndPath As Integer
    Dim StartPath As Integer
    Dim TestDir As Boolean
    
    FormStep1.ProgressBarScan.Max = 3
    FormStep1.ProgressBarScan = 0

    ' ETAPE 1
    FormStep1.LabelActionEnCours = "Lecture des variables d'environnement"
    FormStep1.ProgressBarScan = FormStep1.ProgressBarScan + 1

    ' Verification de la pr�sence des variables d'environnement
    EnvPath = GetRegValue(HKEY_CURRENT_USER, "Environment", "path")
    EndPath = InStr(1, EnvPath, PATH_TO_FIND)
    
    ' Si nous n'avons pas de r�pertoire dans le PATH, nous lisons la variable INCLUDE
    If EndPath < 1 Then
        EnvPath = GetRegValue(HKEY_CURRENT_USER, "Environment", "include")
        EndPath = InStr(1, EnvPath, PATH_TO_FIND)
    End If
        
    ' ETAPE 2
    FormStep1.LabelActionEnCours = "Recherche des r�pertoires"
    FormStep1.ProgressBarScan = FormStep1.ProgressBarScan + 1
    
    ' Recherche des r�pertoires de la OufffLib dans les ENV
    If EndPath > 1 Then
        FormStep1.SelectTask(TASK_REMOVE).Enabled = True
        FormStep1.SelectTask(TASK_RESTORE).Enabled = True
    End If
    
    ' R�cup�ration du nom du r�pertoire
    If EndPath > 1 Then
        StartPath = InStrRev(EnvPath, ";", EndPath - 1)
        Path = Mid(EnvPath, StartPath + 1, (EndPath - (StartPath + 1)))
        FormStep1.Dir.Path = Path
        FormStep1.DirMove.Path = Path
    End If


    ' ETAPE 3
    FormStep1.LabelActionEnCours = "Analyse des r�pertoires"
    FormStep1.ProgressBarScan = FormStep1.ProgressBarScan + 1
    
    ' Si on a un r�pertoire, on verifie le contenu
    If Path <> "" Then
        TestDir = DirExists(Path & PATH_TO_FIND)
        TestDir = TestDir And DirExists(Path & PATH_TO_FIND & "\W32")
        TestDir = TestDir And DirExists(Path & PATH_TO_FIND & "\Linux")
        TestDir = TestDir And DirExists(Path & PATH_TO_FIND & "\Include")
        TestDir = TestDir And DirExists(Path & PATH_TO_FIND & "\Include\Linux")
        TestDir = TestDir And DirExists(Path & PATH_TO_FIND & "\Include\W32")
        TestDir = TestDir And DirExists(Path & PATH_TO_FIND & "\Documentations")
        
        If TestDir = True Then
            ' Tous les r�pertoires existent, on peut deplacer
            FormStep1.SelectTask(TASK_MOVE).Enabled = True
        Else
            FormStep1.SelectTask(TASK_MOVE).Enabled = False
        End If
        
    End If
    
    Call FormStep1.SetActiveStep(1)

End Sub

Public Sub ManageStep()
    Select Case FormStep1.NumStep
        Case 0:
            Call Step0
        
        Case 1:
            If FormStep1.SelectTask(TASK_INSTALL).Value = True Then Call FormStep1.SetActiveStep(21): Call CheckPrevInstall
            If FormStep1.SelectTask(TASK_MOVE).Value = True Then Call FormStep1.SetActiveStep(22)
            If FormStep1.SelectTask(TASK_RESTORE).Value = True Then Call FormStep1.SetActiveStep(23): Call RestoreLib: Call FormStep1.SetActiveStep(43)
            If FormStep1.SelectTask(TASK_REMOVE).Value = True Then Call FormStep1.SetActiveStep(24)
    
        Case 2:
            If FormStep1.SelectTask(TASK_INSTALL).Value = True Then Call FormStep1.SetActiveStep(31)
            If FormStep1.SelectTask(TASK_MOVE).Value = True Then Call FormStep1.SetActiveStep(32)
            If FormStep1.SelectTask(TASK_RESTORE).Value = True Then Call FormStep1.SetActiveStep(33)
            If FormStep1.SelectTask(TASK_REMOVE).Value = True Then Call FormStep1.SetActiveStep(34): Call UnInstallLib: FormStep1.NumStep = FormStep1.NumStep + 1: Call ManageStep
    
        Case 3:
            If FormStep1.SelectTask(TASK_INSTALL).Value = True Then Call FormStep1.SetActiveStep(41): Call InstallLib
            If FormStep1.SelectTask(TASK_MOVE).Value = True Then Call FormStep1.SetActiveStep(42): Call MoveLib
            If FormStep1.SelectTask(TASK_RESTORE).Value = True Then Call FormStep1.SetActiveStep(43)
            If FormStep1.SelectTask(TASK_REMOVE).Value = True Then Call FormStep1.SetActiveStep(44)
    End Select
End Sub



Private Sub InstallLib()
    Dim ReadValue As String

    FormStep1.ProgressBarCurrentAction.Max = 2
    FormStep1.ProgressBarCurrentAction = 0
    
    
    On Error GoTo ErrInstallLibRep
    ' Etape 1 : Cr�ation des r�pertoires
    FormStep1.ProgressBarCurrentAction = FormStep1.ProgressBarCurrentAction + 1
    FormStep1.LabelCurrentAction.Caption = "Cr�ation des r�pertoires"
    
    If Not DirExists(FormStep1.RecapInstallPath.Caption) Then Call MkDir(FormStep1.RecapInstallPath.Caption)
    If Not DirExists(FormStep1.RecapInstallPath.Caption & "W32") Then Call MkDir(FormStep1.RecapInstallPath.Caption & "W32")
    If Not DirExists(FormStep1.RecapInstallPath.Caption & "Linux") Then Call MkDir(FormStep1.RecapInstallPath.Caption & "Linux")
    If Not DirExists(FormStep1.RecapInstallPath.Caption & "Include") Then Call MkDir(FormStep1.RecapInstallPath.Caption & "Include")
    If Not DirExists(FormStep1.RecapInstallPath.Caption & "Include\Linux") Then Call MkDir(FormStep1.RecapInstallPath.Caption & "Include\Linux")
    If Not DirExists(FormStep1.RecapInstallPath.Caption & "Include\W32") Then Call MkDir(FormStep1.RecapInstallPath.Caption & "Include\W32")
    If Not DirExists(FormStep1.RecapInstallPath.Caption & "Documentations") Then Call MkDir(FormStep1.RecapInstallPath.Caption & "Documentations")
    
    
    On Error GoTo ErrInstallLibVar
    ' Etape 2 : Enregistrement des variables d'env
    FormStep1.ProgressBarCurrentAction = FormStep1.ProgressBarCurrentAction + 1
    FormStep1.LabelCurrentAction.Caption = "Enregistrement des variables d'environnement"
    
    ' Verification d'une pr�c�dente install
    ReadValue = GetRegValue(HKEY_CURRENT_USER, "Environment", "OUFFFLIB_PATH")
    If ReadValue <> "" Then
        ' Une pr�c�dente install a �t� trouv�e, on doit vider la base de registre
        ReadValue = GetRegValue(HKEY_CURRENT_USER, "Environment", "path")
        ReadValue = Replace(ReadValue, ";" & FormStep1.RecapInstallPath.Caption & "W32", "")
        If ReadValue <> "" Then Call SetRegValue(HKEY_CURRENT_USER, "Environment", "path", ReadValue)
       
        ReadValue = GetRegValue(HKEY_CURRENT_USER, "Environment", "include")
        ReadValue = Replace(ReadValue, ";" & FormStep1.RecapInstallPath.Caption & "Include\W32", "")
        If ReadValue <> "" Then Call SetRegValue(HKEY_CURRENT_USER, "Environment", "include", ReadValue)
        
        ReadValue = GetRegValue(HKEY_CURRENT_USER, "Environment", "lib")
        ReadValue = Replace(ReadValue, ";" & FormStep1.RecapInstallPath.Caption & "W32", "")
        If ReadValue <> "" Then Call SetRegValue(HKEY_CURRENT_USER, "Environment", "lib", ReadValue)
    End If
    
    ' Enregistrement des nouvelles variables
    Call SetRegValue(HKEY_CURRENT_USER, "Environment", "OUFFFLIB_PATH", FormStep1.RecapInstallPath.Caption)
    
    ReadValue = GetRegValue(HKEY_CURRENT_USER, "Environment", "path")
    If ReadValue <> "" Then
        Call SetRegValue(HKEY_CURRENT_USER, "Environment", "path", ReadValue & ";" & FormStep1.RecapInstallPath.Caption & "W32")
    End If
    
    ReadValue = GetRegValue(HKEY_CURRENT_USER, "Environment", "include")
    If ReadValue <> "" Then
        Call SetRegValue(HKEY_CURRENT_USER, "Environment", "include", ReadValue & ";" & FormStep1.RecapInstallPath.Caption & "Include\W32")
    End If
    
    ReadValue = GetRegValue(HKEY_CURRENT_USER, "Environment", "lib")
    If ReadValue <> "" Then
        Call SetRegValue(HKEY_CURRENT_USER, "Environment", "lib", ReadValue & ";" & FormStep1.RecapInstallPath.Caption & "W32")
    End If
    
    On Error GoTo 0
    GoTo EndInstallLib
    
ErrInstallLibRep:
    On Error GoTo 0
    Call MsgBox("Impossible d'installer la OufffLib. Cr�ation des r�pertoires impossible !", vbOKOnly, "Installation de la OufffLib")
    
    FormStep1.TellMeWhy.Caption = "Il n'est pas possible de cr�er les r�pertoires de la OufffLib." & Chr(13) & " L'installation va se terminer !"
    Call FormStep1.SetActiveStep(99)
    GoTo StopInstall

ErrInstallLibVar:
    On Error GoTo 0
    Call MsgBox("Impossible d'installer la OufffLib. Enregistrement des variables d'environnement impossible !", vbOKOnly, "Installation de la OufffLib")
    
    FormStep1.TellMeWhy.Caption = "Il n'est pas possible d'enregistrer des variables d'environnement." & Chr(13) & " L'installation va se terminer !"
    Call FormStep1.SetActiveStep(99)
    
    GoTo StopInstall
    
StopInstall:
    'L'install ne peut pas etre execut�e, on doit effacer ce qui a �t� fait
    Call UnInstallLib
    GoTo ExitInstallLib

EndInstallLib:
    FormStep1.MsgOK.Caption = "La OufffLib a �t� install�e dans le r�pertoire :" & Chr(13) & FormStep1.RecapInstallPath.Caption & _
    Chr(13) & Chr(13) & "ATTENTION : Pour que tous les modifications soient prises en compte, il faut red�marrer le PC."
    Call FormStep1.SetActiveStep(51)
ExitInstallLib:
End Sub

Private Sub UnInstallLib()
    Dim HomeDir As String
    Dim EnvValue As String
    
    Dim res As Long
    Dim FileOper As SHFILEOPSTRUCT
       
    FormStep1.UninstallProgressBar.Max = 2
    FormStep1.UninstallProgressBar = 0
    
    HomeDir = FindPathName(PATH_TO_FIND)
    
    On Error GoTo ErrUnInstallLibDeleteRep
    ' Etape 1 : Destruction des r�pertoires
    FormStep1.UninstallProgressBar = FormStep1.UninstallProgressBar + 1
    FormStep1.LabelUninstallCurrentAction.Caption = "Destruction des r�pertoires"
    
    FileOper.hWnd = FormStep1.hWnd
    FileOper.wFunc = FO_DELETE
    FileOper.pFrom = HomeDir & PATH_TO_FIND & vbNullChar & vbNullChar
    FileOper.fFlags = FOF_SIMPLEPROGRESS Or FOF_NOCONFIRMATION Or FOF_NOCONFIRMMKDIR Or FOF_SILENT
    res = SHFileOperation(FileOper)

    ' Etape 2 : Destruction des variables d'environement
    FormStep1.UninstallProgressBar = FormStep1.UninstallProgressBar + 1
    FormStep1.LabelUninstallCurrentAction.Caption = "Destruction des variables d'environement"
    
    EnvValue = GetRegValue(HKEY_CURRENT_USER, "Environment", "path")
    EnvValue = Replace(EnvValue, ";" & HomeDir & PATH_TO_FIND & "\W32", "")
    Call SetRegValue(HKEY_CURRENT_USER, "Environment", "path", EnvValue)

    EnvValue = GetRegValue(HKEY_CURRENT_USER, "Environment", "include")
    EnvValue = Replace(EnvValue, ";" & HomeDir & PATH_TO_FIND & "\Include\W32", "")
    Call SetRegValue(HKEY_CURRENT_USER, "Environment", "include", EnvValue)
    
    EnvValue = GetRegValue(HKEY_CURRENT_USER, "Environment", "lib")
    EnvValue = Replace(EnvValue, ";" & HomeDir & PATH_TO_FIND & "\W32", "")
    Call SetRegValue(HKEY_CURRENT_USER, "Environment", "lib", EnvValue)
    
    Call DeleteValue(HKEY_CURRENT_USER, "Environment", "OUFFFLIB_PATH")
    
    GoTo EndUnInstallLib
        
ErrUnInstallLibDeleteRep:
    On Error GoTo ErrUnInstallLibDeleteRep
    Resume Next

EndUnInstallLib:
    On Error GoTo 0
End Sub

Private Function FindPathName(PathToFind As String) As String
    Dim EnvPath As String
    Dim EndPath, StartPath As Integer
    
    On Error GoTo ErrFindPathNameEnv
    ' Verification de la pr�sence des variables d'environnement
    EnvPath = GetRegValue(HKEY_CURRENT_USER, "Environment", "path")
    EndPath = InStr(1, EnvPath, PathToFind)
    
    ' Si nous n'avons pas de r�pertoire dans le PATH, nous lisons la variable INCLUDE
    If EndPath < 1 Then
        EnvPath = GetRegValue(HKEY_CURRENT_USER, "Environment", "include")
        EndPath = InStr(1, EnvPath, PathToFind)
    End If
    
    FindPathName = ""
    
    If EndPath > 1 Then
        StartPath = InStrRev(EnvPath, ";", EndPath)
        FindPathName = Mid(EnvPath, StartPath + 1, EndPath - StartPath - 1)
    End If

    GoTo EndFindPathName
    
ErrFindPathNameEnv:
    On Error GoTo ErrFindPathNameEnv
    Resume Next
    
EndFindPathName:
    On Error GoTo 0
End Function


Private Sub RestoreLib()

    Dim ReadValue As String
    Dim HomePath As String
    
    FormStep1.ProgressBarRep.Max = 2
    FormStep1.ProgressBarRep = 0
    
    ' Verification des variables d'env
    FormStep1.LabelRepEnCours.Caption = "R�paration des variables d'environnement"
    FormStep1.ProgressBarRep = FormStep1.ProgressBarRep + 1

    ReadValue = GetRegValue(HKEY_CURRENT_USER, "Environment", "OUFFFLIB_PATH")
    ReadValue = Replace(ReadValue, "\" & PATH_TO_FIND, "")
    If ReadValue = "" Then
        ' On cherche la lib dans les autres var d'env car OUFFFLIB_PATH n'existe pas
        ' Lecture du r�pertoire
        ReadValue = FindPathName(PATH_TO_FIND)
        If ReadValue = "" Then
            'On a pas trouver le r�pertoire de la OufffLib on sort et on affiche l'erreur
            FormStep1.TellMeWhyErrRep.Caption = "Impossible de d�terminer le r�pertoire o� a �t� install�e la OufffLib." & Chr(13) & "La r�paration est impossible !!"
            Call FormStep1.SetActiveStep(33)
            Exit Sub
        End If
    End If
    HomePath = Replace(ReadValue & "\OufffLib\", "\\", "\")
    
    ' Ecriture des valeurs dans la base de registre
    Call SetRegValue(HKEY_CURRENT_USER, "Environment", "OUFFFLIB_PATH", HomePath)
    
    ReadValue = GetRegValue(HKEY_CURRENT_USER, "Environment", "path")
    ReadValue = Replace(ReadValue, ";" & HomePath & "W32", "")
    ReadValue = ReadValue & ";" & HomePath & "W32"
    Call SetRegValue(HKEY_CURRENT_USER, "Environment", "path", ReadValue)

    ReadValue = GetRegValue(HKEY_CURRENT_USER, "Environment", "lib")
    ReadValue = Replace(ReadValue, ";" & HomePath & "W32", "")
    ReadValue = ReadValue & ";" & HomePath & "W32"
    Call SetRegValue(HKEY_CURRENT_USER, "Environment", "lib", ReadValue)
    
    ReadValue = GetRegValue(HKEY_CURRENT_USER, "Environment", "include")
    ReadValue = Replace(ReadValue, ";" & HomePath & "Include\W32", "")
    ReadValue = ReadValue & ";" & HomePath & "Include\W32"
    Call SetRegValue(HKEY_CURRENT_USER, "Environment", "include", ReadValue)
    
    ' Cr�ation des r�pertoires
    FormStep1.LabelRepEnCours.Caption = "Cr�ation des r�pertoires erron�s"
    FormStep1.ProgressBarRep = FormStep1.ProgressBarRep + 1
    
    If Not DirExists(HomePath) Then Call MkDir(HomePath)
    If Not DirExists(HomePath & "W32") Then Call MkDir(HomePath & "W32")
    If Not DirExists(HomePath & "Linux") Then Call MkDir(HomePath & "Linux")
    If Not DirExists(HomePath & "Include") Then Call MkDir(HomePath & "Include")
    If Not DirExists(HomePath & "Include\Linux") Then Call MkDir(HomePath & "Include\Linux")
    If Not DirExists(HomePath & "Include\W32") Then Call MkDir(HomePath & "Include\W32")
    If Not DirExists(HomePath & "Documentations") Then Call MkDir(HomePath & "Documentations")

    
    FormStep1.LabelRepOk.Caption = "La OufffLib est utilisable dans le r�pertoire : " & Chr(13) & HomePath
End Sub

Private Sub CheckPrevInstall()
    Dim ReadValue As String
    
    ReadValue = GetRegValue(HKEY_CURRENT_USER, "Environment", "OUFFFLIB_PATH")
    If ReadValue <> "" Then
        Call MsgBox("ATTENTION : Une pr�c�dente installation a �t� trouv�e !" & Chr(13) & Chr(13) & "Une nouvelle installation rendra le r�pertoire actuel inutile. Le contenu ne sera plus reconnu par la OufffLib", vbOKOnly, "Attention !")
    End If
    
End Sub

Private Sub MoveLib()

    Dim PrevPath, NewPath, ReadValue As String
    Dim FileOper As SHFILEOPSTRUCT
    Dim res As Long

    On Error GoTo ErrMoveLibRep
        
    FormStep1.ProgressBarCurrentActionMove.Max = 5
    FormStep1.ProgressBarCurrentActionMove = 0
    
    ' R�paration de la Lib avant l'export (pour avoir les bons r�pertoires)
    FormStep1.LabelCurrentActionMove.Caption = "R�paration de la OufffLib avant l'export"
    FormStep1.ProgressBarCurrentActionMove = FormStep1.ProgressBarCurrentActionMove + 1
    Call RestoreLib
    
    ' Lecture du r�pertoire
    FormStep1.LabelCurrentActionMove.Caption = "Lecture du r�pertoire"
    FormStep1.ProgressBarCurrentActionMove = FormStep1.ProgressBarCurrentActionMove + 1
    PrevPath = GetRegValue(HKEY_CURRENT_USER, "Environment", "OUFFFLIB_PATH")

    ' Renomer les r�pertoires
    FormStep1.LabelCurrentActionMove.Caption = "D�placement de la OufffLib"
    FormStep1.ProgressBarCurrentActionMove = FormStep1.ProgressBarCurrentActionMove + 1
    
    PrevPath = Replace(PrevPath, "OufffLib\", "OufffLib")
    NewPath = Replace(FormStep1.LabelMovePathRecap.Caption, "OufffLib\", "OufffLib")
    
    FileOper.hWnd = FormStep1.hWnd
    FileOper.wFunc = FO_COPY
    FileOper.pFrom = PrevPath & vbNullChar & vbNullChar
    FileOper.pTo = NewPath & vbNullChar & vbNullChar
    FileOper.fFlags = FOF_SIMPLEPROGRESS Or FOF_NOCONFIRMATION Or FOF_NOCONFIRMMKDIR Or FOF_SILENT
    res = SHFileOperation(FileOper)
    
    
    ' Modification de la base de registre
    On Error GoTo ErrMoveLibReg
    FormStep1.LabelCurrentActionMove.Caption = "Modification de la base de registre"
    FormStep1.ProgressBarCurrentActionMove = FormStep1.ProgressBarCurrentActionMove + 1
    
    ' Ecriture des valeurs dans la base de registre
    Call SetRegValue(HKEY_CURRENT_USER, "Environment", "OUFFFLIB_PATH", NewPath & "\")
    
    ReadValue = GetRegValue(HKEY_CURRENT_USER, "Environment", "path")
    ReadValue = Replace(ReadValue, PrevPath, NewPath)
    Call SetRegValue(HKEY_CURRENT_USER, "Environment", "path", ReadValue)

    ReadValue = GetRegValue(HKEY_CURRENT_USER, "Environment", "lib")
    ReadValue = Replace(ReadValue, PrevPath, NewPath)
    Call SetRegValue(HKEY_CURRENT_USER, "Environment", "lib", ReadValue)
    
    ReadValue = GetRegValue(HKEY_CURRENT_USER, "Environment", "include")
    ReadValue = Replace(ReadValue, PrevPath, NewPath)
    Call SetRegValue(HKEY_CURRENT_USER, "Environment", "include", ReadValue)

    'On Error GoTo ErrMoveLibDelPrev
    On Error GoTo 0
    FormStep1.LabelCurrentActionMove.Caption = "Suppression de l'ancien r�pertoire"
    FormStep1.ProgressBarCurrentActionMove = FormStep1.ProgressBarCurrentActionMove + 1
    
    FileOper.hWnd = FormStep1.hWnd
    FileOper.wFunc = FO_DELETE
    FileOper.pFrom = PrevPath & vbNullChar & vbNullChar
    FileOper.fFlags = FOF_SIMPLEPROGRESS Or FOF_NOCONFIRMATION Or FOF_NOCONFIRMMKDIR Or FOF_SILENT
    res = SHFileOperation(FileOper)
    
    FormStep1.LabelMsgOKMove.Caption = "La OufffLib a �t� d�plac�e dans le r�pertoire :" & Chr(13) & NewPath & Chr(13) & Chr(13) & _
    "Un red�marrage peut �tre n�cessaire pour mettre � jour les variables syst�mes."
    Call FormStep1.SetActiveStep(52)
    GoTo EndMoveLib
    
ErrMoveLibRep:
    FormStep1.LabelTellMeWhyMove.Caption = "Le r�pertoire destination n'est pas accessible."
    On Error GoTo 0
    Call FormStep1.SetActiveStep(98)
    GoTo EndMoveLib
    
ErrMoveLibReg:
    FormStep1.LabelTellMeWhyMove.Caption = "Impossible de mettre � jour les donn�es syst�me."
    On Error Resume Next
    Call RmDir(NewPath)
    Call FormStep1.SetActiveStep(98)
    GoTo EndMoveLib
    
ErrMoveLibDelPrev:
    FormStep1.LabelTellMeWhyMove.Caption = "La copie est termin�e." & Chr(13) & "L'ancien r�pertoire (" & PrevPath & ") n'a pas pu �tre supprim�." & Chr(13) & Chr(13) & "Vous pouvez l'effacer manuellement."
    Call FormStep1.SetActiveStep(98)
    GoTo EndMoveLib

    
EndMoveLib:
    On Error GoTo 0
End Sub
