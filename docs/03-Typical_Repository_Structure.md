# Typical Repository Structure

***REQUIRED:*** `packages.json` - houses package names and their checksums

package.json
```json
{
    "uspm":"199833526",
    "cjson":"284238349"
}
```

The rest are basically just .uspm packages that you'd want on the server.
```
.
+-- cjson.uspm
+-- libcurl.uspm
+-- openssl.uspm
+-- packages.json
+-- tar-tbc.uspm
+-- uspm.uspm
+-- uspp-src.uspm
```

If you'd like you can keep archived versions of packages in the `archive` folder. 
However, the USPM standard does not require that such a folder exist.