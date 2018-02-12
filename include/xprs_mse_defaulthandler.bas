Attribute VB_Name = "Xpress_MipSolEnum_DefaultHandler"
' Module XPRS_MSE_DEFAULTHANDLER.BAS       (c) Fair Isaac Corporation 2008
' Declares default handler for use with XPRSmipsolenum

Option Explicit
Option Compare Text

Public Function XPRS_mse_defaulthandler(ByVal mse As Long, ByVal prob As Long, ByVal msp As Long, ByRef ctx As String, ByRef nMaxSols As Long, ByVal x_Zb_pointer As Long, ByVal nCols As Integer, ByVal dMipObject As Double, ByRef dModifiedObject As Double, ByRef bRejectSoln As Long, ByRef bUpdateMipAbsCutOffOnCurrentSet As Long) As Long
    Dim nDeleted As Long
    Dim bStorageIsFullOnEntry As Long
    Dim cull_sol_id_list() As Long
    nDeleted = 0
    bStorageIsFullOnEntry = 0
    
    Dim nMaxSolsToCull_MIPOBJECT As Long, nMaxSolsToCull_DIVERSITY As Long
    nMaxSolsToCull_MIPOBJECT = -1
    nMaxSolsToCull_DIVERSITY = -1
    
    If (nMaxSols > 0) Then
        Dim nSoln As Long
        If (XPRS_mse_getintattrib(mse, XPRS_MSE_SOLUTIONS, nSoln) <> 0) Then GoTo mse_error
        
        If nSoln >= nMaxSols Then
            ' Keeping the new solution will put the number of solutions above the max limit.
            ' Either choose to ignore the new solution and/or delete some solutions already
            ' stored.
            Dim i As Long, nSolsToCull As Long, iSolutionIdStatus As Long, nMaxSolsToCull As Long
            nMaxSolsToCull = 1
            bStorageIsFullOnEntry = 1
            
            ' Get some parameters to use to run the routine calls here
            If (XPRS_mse_getintcontrol(mse, XPRS_MSE_CALLBACKCULLSOLS_MIPOBJECT, nMaxSolsToCull_MIPOBJECT) <> 0) Then GoTo mse_error
            If (nMaxSolsToCull < nMaxSolsToCull_MIPOBJECT) Then
                nMaxSolsToCull = nMaxSolsToCull_MIPOBJECT
            End If
            If (XPRS_mse_getintcontrol(mse, XPRS_MSE_CALLBACKCULLSOLS_DIVERSITY, nMaxSolsToCull_DIVERSITY) <> 0) Then GoTo mse_error
            If (nMaxSolsToCull < nMaxSolsToCull_DIVERSITY) Then
                nMaxSolsToCull = nMaxSolsToCull_DIVERSITY
            End If
            
            ' Allocate an array to store the list of existing solutions we may be culling here
            ReDim cull_sol_id_list(0 To nMaxSolsToCull - 1)
            
            ' Try culling some existing solutions and maybe choose to ignore the new solution as well
            If (nMaxSolsToCull_MIPOBJECT >= 0) Then
                If (XPRS_mse_getcullchoice(mse, XPRS_MSE_METRIC_MIPOBJECT, cull_sol_id_list(0), nMaxSolsToCull_MIPOBJECT, nSolsToCull, dMipObject, 0, 0, bRejectSoln) <> 0) Then GoTo mse_error
                For i = 1 To nSolsToCull
                    If (XPRS_msp_delsol(msp, cull_sol_id_list(i - 1), iSolutionIdStatus) <> 0) Then GoTo msp_error
                    nDeleted = nDeleted + 1
                Next
            End If
            If (nMaxSolsToCull_DIVERSITY >= 0) Then
                If (XPRS_mse_getcullchoice(mse, XPRS_MSE_METRIC_DIVERSITY, cull_sol_id_list(0), nMaxSolsToCull_MIPOBJECT, nSolsToCull, dMipObject, 0, 0, bRejectSoln) <> 0) Then GoTo mse_error
                For i = 1 To nSolsToCull
                    If (XPRS_msp_delsol(msp, cull_sol_id_list(i - 1), iSolutionIdStatus) <> 0) Then GoTo msp_error
                    nDeleted = nDeleted + 1
                Next
            End If
            
            If (bRejectSoln = 0 And nDeleted = 0) Then
                ' None of the policies above were able to handle the current situation
                ' We need to either ignore the new solution or delete an existing solution
                ' to make way for the new solution.  Use the mip objective to decide
                ' the worst solution
                nMaxSolsToCull_MIPOBJECT = 1
                If (XPRS_mse_getcullchoice(mse, XPRS_MSE_METRIC_MIPOBJECT, cull_sol_id_list(0), nMaxSolsToCull_MIPOBJECT, nSolsToCull, dMipObject, 0, 0, bRejectSoln) <> 0) Then GoTo mse_error
                If (bRejectSoln <> 0 Or nSolsToCull = 0) Then
                    ' We still haven't rejected any solutions
                    nMaxSolsToCull_MIPOBJECT = 0
                Else
                    ' The new solution is better than the worst stored solution
                    If (XPRS_msp_delsol(msp, cull_sol_id_list(0), iSolutionIdStatus) <> 0) Then GoTo msp_error
                    nDeleted = nDeleted + 1
                End If
            End If
        End If
    End If
    
    If (bStorageIsFullOnEntry <> 0) Then
        If ((nDeleted = 1) Xor (bRejectSoln <> 0)) Then
            ' We haven't reduced the number of solutions in storage and the storage is full
            If (nMaxSolsToCull_MIPOBJECT >= 0) Then
                ' The storage is full and we are using the mip objective as a metric for managing
                ' the stored solutions.  Update the mip cut-off to reflect the worst solution we
                ' have stored (or that we will have stored once the new solution is loaded when we
                ' return from here)
                bUpdateMipAbsCutOffOnCurrentSet = 1
            End If
        End If
    End If
    
    XPRS_mse_defaulthandler = 0
    Exit Function
    
mse_error:
    Dim mse_err_msg As String
    Dim mse_err_msg_code As Long, mse_err_msg_len As Long
    XPRS_mse_getlasterror mse, mse_err_msg_code, mse_err_msg, 255, mse_err_msg_len
    MsgBox mse, vbCritical, "XPRSmipsolenum error"
    XPRS_mse_defaulthandler = 1
    Exit Function

msp_error:
    Dim msp_err_msg As String
    Dim msp_err_msg_code As Long, msp_err_msg_len As Long
    XPRS_msp_getlasterror mse, msp_err_msg_code, msp_err_msg, 255, msp_err_msg_len
    MsgBox msp, vbCritical, "XPRSmipsolenum error"
    XPRS_mse_defaulthandler = 1
    Exit Function
End Function

