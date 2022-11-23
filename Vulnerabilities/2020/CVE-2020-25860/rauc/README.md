# Integrating RAUC 1.5

This repository contains examples to simplify integrating the RAUC update into
existing projects. You can subscribe to
https://github.com/rauc/rauc-1.5-integration/issues/1 to receive notifications
of important updates to this repository and of integration into the upstream
build systems.

The examples have been derived from the current upstream versions, so some
adjustment may be needed depending on the age of the existing project.

The advisory to be published on the 21st December is available as
https://github.com/rauc/rauc-1.5-integration/blob/master/CVE-2020-25860.md.
This repository will also be made public at that point.

## OpenEmbedded/Yocto

If the ``rauc-1.5.tar.xz`` archive is not available for download yet, copy the
archive from this repository to your ``DL_DIR`` (usually defined in your
``build/conf/local.conf``).

Copy the updated recipe from ``oe_yocto/recipes-rauc-1.5/rauc/*`` to the appropriate
project-specific layer, taking care to keep existing customization (such as
``system.conf`` or ``ca.cert.pem``).

Copy ``oe_yocto/classes/verity_bundle.bbclass`` to the ``classes/`` directory of
this layer (you may need to create that directory). This class is a replacement
for the ``bundle.bbclass`` from the meta-rauc layer, with added support for
creating bundles in the ``verity`` format. The different name is used to avoid
issues with class search order.

If you want to build bundles in the ``verity`` format, use ``inherit verity_bundle``
instead of ``inherit bundle`` in your bundle recipe and set ``RAUC_BUNDLE_FORMAT
= "verity"`` to select the format. An example is available as
``oe_yocto/recipes-rauc-1.5/bundles/core-bundle-minimal-verity.bb``.

## PTXdist

If the ``rauc-1.5.tar.xz`` archive is not available for download yet, copy the
archive from this repository to your PTXdist source directory.

Copy ``ptxdist/rules/rauc.*`` directory to ``rules/`` in your BSP. This results
in an updated RAUC binary for the target and the host.

If you use the default RAUC bundle rule from PTXdist, copy
``ptxdist/config/images/rauc.config`` to ``config/images/`` in your BSP. The
included manifest template configures the ``plain`` bundle format, so change
this as needed.

If you use a custom bundle and want to change to the ``verity`` format, you will
need to update the manifest for it yourself.

## Buildroot

If the ``rauc-1.5.tar.xz`` archive is not available for download yet, copy the
archive from this repository to your ``dl/rauc/`` directory.

Copy ``buildroot/package/rauc/rauc.*`` to ``package/rauc/`` in your Buildroot
checkout. This results in an updated RAUC binary for the target and the host.

As Buildroot doesn't automate building update bundles, if you want to use the
``verity`` bundle format, you will need to update your manifest manually.

## OpenSSL < 1.1.1

RAUC v1.3 (released on 2020-04-23) removed support for OpenSSL versions before
1.1.1, as they were no longer supported by the OpenSSL project:

* 1.1.0 was supported until 2019-09-11
* 1.0.2 was supported until 2019-12-31 as an LTS relase

If your system still uses an unspported version of OpenSSL and updating to
1.1.1 together with RAUC 1.5 is not feasible, a backport patch
(``0001-backport-to-OpenSSL-1.0.2.patch``) is available. Note that this patch
does not have the same breadth of testing as the 1.5 release.

## Build with Kernel (Headers) < 4.14

In RAUC 1.5, the loop device ioctl ``LOOP_SET_BLOCK_SIZE`` is used to set the
logical block size to 4096 for performance optimization.
Not being able to set it is not critical and only noted by a debug level
message.

But as the ``LOOP_SET_BLOCK_SIZE`` symbol does not exists in kernel versions
prior to 4.14, compilation with against these older kernel headers will fail.
To resolve this, the symbol must optionally be defined in RAUC itself.
A patch for this (``0001-src-mount.c-fix-build-with-kernel-4.14.patch``) is
available (from https://github.com/rauc/rauc/pull/673).

The fix will also be part of RAUC
[1.5.1](https://github.com/rauc/rauc/milestone/12).
