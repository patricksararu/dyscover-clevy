<?xml version="1.0"?>
<!-- Copyright (c) 2001-2007 Nuance Communications, Inc. -->
<!-- All Rights Reserved -->

<xsl:stylesheet version="1.0" xmlns:err="http://www.nuance.com/vocalizer50/ErrorMessages" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method="html"/>

<xsl:template match="/">
  <html>
    <head>
      <title>Nuance Vocalizer Errors</title>
    </head>
    <body bgcolor="#ffe4bf">
      <xsl:apply-templates select="//err:ErrorMessages"/>
    </body>
  </html>
</xsl:template>

<xsl:template match="err:ErrorMessages">
  <h1>Nuance Vocalizer&#8482; Errors</h1>
  <p> 
    Copyright &#169; 2004-2007 Nuance Communications, Inc. <br/>
    All Rights Reserved
  </p>

  <p>
    <a href="#Error0">Errors 0 - 999</a>
    <br/>
    <xsl:for-each select="err:error">
      <xsl:if test="@num &gt; 1">
      <xsl:if test="@num mod 1000 = 0">
        <xsl:element name="a">
          <xsl:attribute name="href">#Error<xsl:value-of select="@num"/>
          </xsl:attribute>
          Errors <xsl:value-of select="@num"/> -
          <xsl:value-of select="@num + 999"/>
        </xsl:element>
        <br/>
      </xsl:if>
      </xsl:if>
    </xsl:for-each>
  </p>

  <p>
    <a href="http://support.nuance.com">
      Nuance Technical Support
    </a>
  </p>

  <h2><a name="#Error0">Errors 0 - 999</a></h2>
  <table border="5" cellspacing="2" cellpadding="1">
    <tr>
      <td><center><em>Error Number</em></center></td>
      <td><center><em>Severity</em></center></td>
      <td><center><em>Error Text</em></center></td>
    </tr>
    <xsl:apply-templates select="err:error"/>
  </table>
</xsl:template>

<xsl:template match="err:error">
  <xsl:if test="@num &gt; 1">
  <xsl:if test="@num mod 1000 = 0">
    <xsl:text disable-output-escaping="yes">&lt;/table&gt;</xsl:text>
    <h2>
      <xsl:element name="a">
        <xsl:attribute name="name">#Error<xsl:value-of select="@num"/>
        </xsl:attribute>
        Errors <xsl:value-of select="@num"/> -
        <xsl:value-of select="@num + 999"/>
      </xsl:element>
    </h2>
    <xsl:text disable-output-escaping="yes">
    &lt;table border="5" cellspacing="2" cellpadding="1"&gt;
    </xsl:text>
      <tr>
      <td><center><em>Error Number</em></center></td>
      <td><center><em>Severity</em></center></td>
      <td><center><em>Error Text</em></center></td>
      </tr>
  </xsl:if>
  </xsl:if>

  <tr>
    <td><xsl:value-of select="@num"/></td>
    <td>
      <xsl:choose>
        <xsl:when test="contains(@severity,'1')">Critical</xsl:when>
        <xsl:when test="contains(@severity,'2')">Severe</xsl:when>
        <xsl:when test="contains(@severity,'3')">Warning</xsl:when>
        <xsl:when test="contains(@severity,'4')">Info</xsl:when>
        <xsl:when test="contains(@severity,'5')">Disabled</xsl:when>
        <xsl:otherwise>@severity</xsl:otherwise>
      </xsl:choose>
   </td>
   <td><xsl:value-of select="text()"/> </td>
  </tr>
</xsl:template>

</xsl:stylesheet>
