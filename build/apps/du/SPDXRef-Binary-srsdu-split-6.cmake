
			cmake_policy(SET CMP0011 NEW)
			cmake_policy(SET CMP0012 NEW)
			set(_install_root "$ENV{DESTDIR}/usr/local")
			if(NOT EXISTS "${_install_root}/bin/srsdu")
				if(NOT FALSE)
					message(FATAL_ERROR "Cannot find bin/srsdu")
				endif()
			else()
				file(SHA1 ${_install_root}/bin/srsdu _sha1)
				list(APPEND SBOM_VERIFICATION_CODES ${_sha1})
				file(APPEND "/home/ubuntu/test/srsRAN_Project/build/sbom/sbom.spdx.in"
"
FileName: ./bin/srsdu
SPDXID: SPDXRef-Binary-srsdu-split-6
FileType: BINARY
FileChecksum: SHA1: ${_sha1}
LicenseConcluded: LicenseRef-srsRAN
LicenseInfoInFile: NOASSERTION
FileCopyrightText: NOASSERTION
"
				)
			endif()
			