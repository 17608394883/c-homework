;������ֳ���
#define MyAppName "ApplicationUpdateService"

;����İ汾�ţ���˾���ƣ���ַ�� �������->ж��Ӧ�ó��� �ĵط��ɿ���
#define MyAppVersion "latest"
#define MyAppPublisher "�����ʵ��ѧ"
#define MyAppURL "http://www.cqupt.edu.cn/"

#define MyAppExeName "ApplicationUpdateService.exe"

#define VCmsg "Installing Microsoft Visual C++ Redistributable...."

#define BuildYear GetDateTimeString('yyyy', '', '');
#define BuildMonth GetDateTimeString('mm', '', '');
#define BuildDay GetDateTimeString('dd', '', '');
#define BuildHour GetDateTimeString('hh', '', '');
[Setup]
; NOTE: The value of AppId uniquely identifies this application. Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{42C7F08D-A6EF-47F1-BA49-4FEB77E0F6CE}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppVerName={#MyAppName}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}

;��װ���ļ����Ŀ¼
OutputDir=..\build\{#MyAppName}

;��װ���ļ���
OutputBaseFilename={#MyAppName}-{#MyAppVersion}

;ѹ���ļ�
Compression=lzma
SolidCompression=yes

;�Թ���ԱȨ������
PrivilegesRequired=admin

VersionInfoVersion={#BuildYear}.{#BuildMonth}.{#BuildDay}.{#BuildHour}


[Languages]
Name: "chinesesimplified"; MessagesFile: "compiler:Languages\ChineseSimplified.isl"

[Files]
; for cert
Source: "..\deps\MicrosoftRootCertificateAuthority2011.cer"; DestDir: {tmp}
; for vc
Source: "..\deps\vcredist_x86.exe"; DestDir: {tmp}; Flags: deleteafterinstall
; Update Service
Source: "..\Release\*"; Excludes: "*.pdb,*.exp,log,downloads"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
; no icon

[Run]
; for cert
Filename: {sys}\certutil; Parameters: " -addstore root {tmp}\MicrosoftRootCertificateAuthority2011.cer";
; for vc runtime
Filename: "{tmp}\vcredist_x86.exe"; Parameters: "/install /quiet /norestart" ;StatusMsg: "{#VCmsg}"; Check: VCRedistNeedsInstall
; Update Service
Filename: {app}\ApplicationUpdateService.exe; Parameters: "-install" ; Flags: runhidden
Filename: {sys}\sc.exe; Parameters: "start GenericAppUpdateService" ; Flags: runhidden

[UninstallRun]
; Update Service
Filename: {sys}\sc.exe; Parameters: "stop GenericAppUpdateService" ; Flags: runhidden
Filename: {sys}\sc.exe; Parameters: "delete GenericAppUpdateService" ; Flags: runhidden

[Registry]
Root: HKCR; Subkey: "UNTECH\{{42C7F08D-A6EF-47F1-BA49-4FEB77E0F6CE}"; ValueType: string; ValueName: "build_date"; ValueData: "{#BuildYear}.{#BuildMonth}.{#BuildDay}.{#BuildHour}"

[Code]
#include "vc.iss"
function InitializeSetup(): boolean;
var
  ResultStr: String;
  ResultCode: Integer;
begin
  if RegQueryStringValue(HKLM, 'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{42C7F08D-A6EF-47F1-BA49-4FEB77E0F6CE}_is1', 'UninstallString', ResultStr) then
    begin
      ResultStr := RemoveQuotes(ResultStr);
      Exec(ResultStr, '/silent', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
    end;
    result := true;
  if RegQueryStringValue(HKLM, 'SOFTWARE\WOW6432Node\Microsoft\Windows\CurrentVersion\Uninstall\{42C7F08D-A6EF-47F1-BA49-4FEB77E0F6CE}_is1', 'UninstallString', ResultStr) then
    begin
      ResultStr := RemoveQuotes(ResultStr);
      Exec(ResultStr, '/silent', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
    end;
    result := true;
end;